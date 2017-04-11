#include "config.hpp"

#include "globalVars.hpp"

Camera::Camera()
{
  this->upVector = vec3(0.0f, 1.0f, 0.0f);
  this->thetax = 90.0;
  this->thetay = 135.0;

  this->fov = 45.0f;

  this->projectionMatrix = perspective(this->fov, 1024.0f / 768.0f, 0.1f, 100.0f);
}

void Camera::rotateAroundTarget(double xTheta)
{
  GLfloat r = length(vec3(this->target.x - this->position.x, 0.0f, this->target.z - this->position.z));

  this->thetax = (int) ceil(this->thetax + 6 * xTheta) % 360;
  GLfloat x = this->target.x + r * cos(3.1415925f / 180.0f * this->thetax);
  GLfloat z = this->target.z + r * sin(3.1415925f / 180.0f * this->thetax);

  this->position = vec3(x, this->position.y, z);
  this->viewMatrix = glm::lookAt(this->position, this->target, this->upVector);
}

void Camera::zoom(double val)
{
  if(this->fov >= 44.1f && this->fov <= 47.0f)
    this->fov -= val;
  if(this->fov <= 44.1f)
    this->fov = 44.1f;
  if(this->fov >= 47.0f)
    this->fov = 47.0f;

  cout << this->fov << endl;

  this->projectionMatrix = perspective(this->fov, 1024.0f / 768.0f, 0.1f, 100.0f);
}

void Camera::look(vec3 position, vec3 target, vec3 upVector)
{
  this->position = position;
  this->target = target;
  this->upVector = upVector;

  this->viewMatrix = glm::lookAt(this->position, this->target, this->upVector);
}

mat4 Camera::getProjectionMatrix()
{
  return this->projectionMatrix;
}

mat4 Camera::getViewMatrix()
{
  return this->viewMatrix;
}
