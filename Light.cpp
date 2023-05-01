#include "Light.h"

Light::Light(std::string lightId, std::string type, glm::vec3 color, float intensity)
    : m_type(type), m_color(color), m_intensity(intensity), m_lightId(lightId)
{
}

void Light::setType(std::string type)
{
    m_type = type;
}

std::string Light::getType() const
{
    return m_type;
}

void Light::setColor(glm::vec3 color)
{
    m_color = color;
}

glm::vec3 Light::getColor() const
{
    return m_color;
}

void Light::setIntensity(float intensity)
{
    m_intensity = intensity;
}

float Light::getIntensity() const
{
    return m_intensity;
}

void Light::setPosition(glm::vec3 position)
{
    m_position = position;
}

glm::vec3 Light::getPosition() const
{
    return m_position;
}

void Light::setDirection(glm::vec3 direction)
{
    m_direction = direction;
}

glm::vec3 Light::getDirection() const
{
    return m_direction;
}

void Light::setRotation(glm::vec3 rotation)
{
  m_rotation = rotation;
}

glm::vec3 Light::getRotatio() const
{
  return m_rotation;
}

void Light::setCutoff(float cutoff)
{
    m_cutoff = cutoff;
}

float Light::getCutoff() const
{
    return m_cutoff;
}

void Light::setOuterCutoff(float outerCutoff)
{
    m_outerCutoff = outerCutoff;
}

float Light::getOuterCutoff() const
{
    return m_outerCutoff;
}

std::string Light::getLightId() const
{
    return m_lightId;
}

void Light::setShader(ShaderProgram* shaderProgram)
{
  m_shaderProgram = shaderProgram;
}

ShaderProgram* Light::getShader()
{
  return m_shaderProgram;
}


void Light::updateUniforms() const
{
  if(m_type == "Directional")
  {
      m_shaderProgram->setVec3("light.direction", m_direction);
      m_shaderProgram->setVec3("light.color", m_color);
      m_shaderProgram->setFloat("light.intensity", m_intensity);
  }
  else if(m_type == "Point")
  {
      m_shaderProgram->setVec3("lightPosition", m_position);
      m_shaderProgram->setVec3("lightColor", m_color);
      m_shaderProgram->setFloat("lightIntensity", m_intensity);
  }
  else if(m_type == "Spot")
  {
      m_shaderProgram->setVec3("light.position", m_position);
      m_shaderProgram->setVec3("light.direction", m_direction);
      m_shaderProgram->setVec3("light.color", m_color);
      m_shaderProgram->setFloat("light.intensity", m_intensity);
      m_shaderProgram->setFloat("light.cutoff", m_cutoff);
      m_shaderProgram->setFloat("light.outerCutoff", m_outerCutoff);
  }
  else if(m_type == "Area")
  {
      m_shaderProgram->setVec3("light.position", m_position);
      m_shaderProgram->setVec3("light.rotation", m_rotation);
      m_shaderProgram->setVec3("light.color", m_color);
  }

}

void Light::draw(const glm::mat4& view, const glm::mat4& projection) const
{
//  m_shaderProgram->use();

//  updateUniforms();

  /*m_shaderProgram->setMat4("view", view);
  m_shaderProgram->setMat4("projection", projection);*/

  if(m_type == "Directional")
  {
      // Set up the directional light.
      GLfloat position[] = {m_direction.x, m_direction.y, m_direction.z, 0.0f};
      GLfloat color[] = {m_color.r, m_color.g, m_color.b, 1.0f};
      glLightfv(GL_LIGHT0, GL_POSITION, position);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
      glLightfv(GL_LIGHT0, GL_SPECULAR, color);
      glEnable(GL_LIGHT0);
  }
  else if(m_type == "Point")
  {
      // Set up the point light.
      GLfloat position[] = {m_position.x, m_position.y, m_position.z, 1.0f};
      GLfloat color[] = {m_color.r, m_color.g, m_color.b, 1.0f};
      glLightfv(GL_LIGHT1, GL_POSITION, position);
      glLightfv(GL_LIGHT1, GL_DIFFUSE, color);
      glLightfv(GL_LIGHT1, GL_SPECULAR, color);
      glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
      glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
      glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);
      glEnable(GL_LIGHT1);
  }
  else if(m_type == "Spot")
  {
      // Set up the spot light.
      GLfloat position[] = {m_position.x, m_position.y, m_position.z, 1.0f};
      GLfloat color[] = {m_color.r, m_color.g, m_color.b, 1.0f};
      GLfloat direction[] = {m_direction.x, m_direction.y, m_direction.z};
      glLightfv(GL_LIGHT2, GL_POSITION, position);
      glLightfv(GL_LIGHT2, GL_DIFFUSE, color);
      glLightfv(GL_LIGHT2, GL_SPECULAR, color);
      glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, m_cutoff);
      glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direction);
      glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
      glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.0f);
      glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.0f);
      glEnable(GL_LIGHT2);
  }
  else if(m_type == "Area")
  {
      glPushMatrix();
      glTranslatef(m_position.x, m_position.y, m_position.z);
      glRotatef(m_rotation.x, 1.0f, 0.0f, 0.0f);
      glRotatef(m_rotation.y, 0.0f, 1.0f, 0.0f);
      glRotatef(m_rotation.z, 0.0f, 0.0f, 1.0f);
      float size = (m_areaSize.x + m_areaSize.y) / 2.0f;
      glBegin(GL_QUADS);
      glColor3f(m_color.r, m_color.g, m_color.b);
      glVertex3f(-size, 0.0f, -size);
      glVertex3f(size, 0.0f, -size);
      glVertex3f(size, 0.0f, size);
      glVertex3f(-size, 0.0f, size);
      glEnd();
      glPopMatrix();
  }

  //m_shaderProgram->unuse();
}
