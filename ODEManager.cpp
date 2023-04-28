#include "ODEManager.h"

dWorldID ODEManager::world = dWorldCreate();
dJointGroupID ODEManager::contactgroup = dJointGroupCreate(0);

ODEManager::ODEManager() {
    // Initialize ODE
    dInitODE();

    // Create a world
    dWorldSetGravity(world, 0, -9.81, 0);

    // Create a space
    space = dHashSpaceCreate(0);

    // Create a contact group

}

ODEManager::~ODEManager() {
    cleanup();
}

void ODEManager::update(float deltaTime) {
    // Step the simulation forward in time
    dSpaceCollide(space, this, &ODEManager::nearCallback);
    dWorldQuickStep(world, deltaTime); // changed to quick step for better performance

    // Clear the contact group
    dJointGroupEmpty(contactgroup);
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
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    if (b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact)) return;

    const int N = 10; // maximum number of contact points
    dContact contact[N];

    int numContacts = dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));
    if (numContacts > 0) {
        for (int i = 0; i < numContacts; ++i) {
            contact[i].surface.mode = dContactBounce | dContactSoftCFM;
            contact[i].surface.mu = dInfinity;
            contact[i].surface.bounce = 0.2;
            contact[i].surface.bounce_vel = 0.1;
            contact[i].surface.soft_cfm = 0.01;

            dJointID c = dJointCreateContact(world, contactgroup, &contact[i]);
            dJointAttach(c, b1, b2);
        }
    }
}

void ODEManager::cleanup() {
    // Destroy all objects
    for (auto object : objects) { // changed to range-based for loop
        delete object;
    }
    objects.clear();

    // Destroy the contact joint group
    dJointGroupDestroy(contactgroup);

    // Destroy the space
    dSpaceDestroy(space);

    // Destroy the world
    dWorldDestroy(world);

    // Close ODE
    dCloseODE();
}
