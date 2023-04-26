#pragma once

#include <ode/ode.h>
#include <vector>

class ODEManager {
public:
    ODEManager();
    ~ODEManager();
    void init();
    void cleanup();
    void update(double deltaTime);
    dWorldID getWorldID() const;
    dJointGroupID getContactJointGroupID() const;
    void addObject(dGeomID geom, dReal mass);
    void removeObjects();

private:
    void nearCallback(void* data, dGeomID o1, dGeomID o2);

    dWorldID worldID;
    dSpaceID spaceID;
    dJointGroupID contactJointGroupID;
    std::vector<dBodyID> bodyIDs;
    std::vector<dGeomID> geomIDs;
    std::vector<dMass> masses;
    int objectCount;
};
