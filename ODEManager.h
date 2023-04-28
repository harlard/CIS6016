#ifndef ODE_MANAGER_H
#define ODE_MANAGER_H

#include <vector>
#include <algorithm>
#include <ode/ode.h>
#include <ode/contact.h>
#include "ODEObject.h"

class ODEManager {
public:
    ODEManager();
    ~ODEManager();
    void update(float dt);
    static void nearCallback(void* data, dGeomID o1, dGeomID o2);
    void addObject(ODEObject* object);
    void removeObject(ODEObject* object);
    void cleanup();

private:
    static dWorldID world;
    static dSpaceID space;
    static dJointGroupID contactgroup;
    std::vector<ODEObject*> objects;
};

#endif // ODE_MANAGER_H
