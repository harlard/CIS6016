#ifndef ODEOBJECT_H
#define ODEOBJECT_H

#include <ode/ode.h>
#include <glm/glm.hpp>

class ODEObject {
public:
    ODEObject(dWorldID world, dSpaceID space);
    ~ODEObject();

    dBodyID getBody() const { return m_body; }
    dGeomID getGeom() const { return m_geom; }

    void setPosition(glm::vec3 position);
    void setRotation(glm::mat3 rotation);
    void setMass(float mass);
    void setGeom(dGeomID geom);

private:
    dWorldID m_world;
    dSpaceID m_space;
    dBodyID m_body;
    dGeomID m_geom;
    float m_mass;
};

#endif
