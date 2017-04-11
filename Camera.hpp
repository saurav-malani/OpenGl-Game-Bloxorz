#include "config.hpp"

#ifndef CAMERA
#define CAMERA

class Camera {

public:
  mat4 viewMatrix, projectionMatrix;
  GLfloat thetax, thetay, fov;
  vec3 position, target, upVector;

  Camera();

  void look(vec3 position, vec3 target, vec3 upVector);
  void rotateAroundTarget(double xTheta);
  void zoom(double val);

  mat4 getProjectionMatrix();
  mat4 getViewMatrix();

};

#endif
