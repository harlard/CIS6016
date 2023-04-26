#include "ODEManager.h"

// Define a global variable to store the world ID
static dWorldID world;

ODEManager::ODEManager() {
    // Initialize ODE
    dInitODE();

    // Create a world
    world = dWorldCreate();
    dWorldSetGravity(world, 0, -9.81, 0);

    // Create a space
    space = dHashSpaceCreate(0);

    // Create a contact group
    contactGroup = dJointGroupCreate(0);
}

ODEManager::~ODEManager() {
    cleanup();
}

void ODEManager::update(float deltaTime) {
    // Step the simulation forward in time
    dSpaceCollide(space, this, &ODEManager::nearCallback);
    dWorldStep(world, deltaTime);

    // Clear the contact group
    dJointGroupEmpty(contactGroup);
}

void ODEManager::addObject(ODEObject* object) {
    objects.push_back(object);
}

void ODEManager::removeObject(ODEObject* object) {
    for (auto iter = objects.begin(); iter != objects.end(); ++iter) {
        if (*iter == object) {
            objects.erase(iter);
            break;
        }
    }
}

void ODEManager::nearCallback(void* data, dGeomID o1, dGeomID o2) {
    // Check if the objects are part of the same body
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    if (b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact)) {
        return;
    }

    // Create a contact joint between the two objects
    dContact contact;
    contact.surface.mode = dContactBounce | dContactSoftCFM;
    contact.surface.mu = dInfinity;
    contact.surface.bounce = 0.2;
    contact.surface.bounce_vel = 0.1;
    contact.surface.soft_cfm = 0.001;
    if (int numContacts = dCollide(o1, o2, 1, &contact.geom, sizeof(dContact))) {
        dBodyID b1 = dGeomGetBody(o1);
        dBodyID b2 = dGeomGetBody(o2);
        dJointID c = dJointCreateContact(world, contactGroup, &contact);
        dJointAttach(c, b1, b2);
    }
}

void ODEManager::cleanup() {
    // Destroy all objects
    for (auto object : objects) {
        delete object;
    }
    objects.clear();

    // Destroy the contact group
    dJointGroupDestroy(contactGroup);

    // Destroy the space
    dSpaceDestroy(space);

    // Destroy the world
    dWorldDestroy(world);

    // Close ODE
    dCloseODE();
}
