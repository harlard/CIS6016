#include <ode/ode.h>
#include "ODEObject.h"

ODEObject::ODEObject(dWorldID world, dSpaceID space)
    : m_world(world), m_space(space), m_geom(nullptr), m_body(nullptr), m_mass(0.0f)
{
    m_body = dBodyCreate(world);
}

ODEObject::~ODEObject()
{
    if (m_geom != nullptr) {
        dGeomDestroy(m_geom);
    }
    if (m_body != nullptr) {
        dBodyDestroy(m_body);
    }
}

void ODEObject::setPosition(glm::vec3 position)
{
    dBodySetPosition(m_body, position.x, position.y, position.z);
}

void ODEObject::setRotation(glm::mat3 rotation)
{
    dMatrix3 mat;
    mat[0] = rotation[0][0];
    mat[1] = rotation[1][0];
    mat[2] = rotation[2][0];
    mat[3] = rotation[0][1];
    mat[4] = rotation[1][1];
    mat[5] = rotation[2][1];
    mat[6] = rotation[0][2];
    mat[7] = rotation[1][2];
    mat[8] = rotation[2][2];
    dBodySetRotation(m_body, mat);
}

void ODEObject::setMass(float mass)
{
  if (m_geom == nullptr) {
      return;
  }

  dMass m;
  dMassSetBoxTotal(&m, mass, 1, 1, 1);
  dGeomSetBody(m_geom, m_body);
  dMassAdjust(&m, mass);
  dBodySetMass(m_body, &m);
}

void ODEObject::setGeom(dGeomID geom)
{
    m_geom = geom;
    dGeomSetBody(m_geom, m_body);
}
