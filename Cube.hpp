#include "config.hpp"

#include "shader.hpp"
#include "3DObject.hpp"
#include "Grid.hpp"

#ifndef CUBE
#define CUBE


class Cube {

private:
  Shader *shader;
  Model *cube;

public:
  bool turning, falling;
  GLfloat width, height, depth;
  GLfloat turnRotation;
  GLint mulFac, moves;
  vec3 centerPosition, topCenterPosition, bottomCenterPosition, instCenterPosition, rotationAxis, transform, invTransform;
  Grid *grid;

  Cube(Shader *shader, GLchar *path, Grid *onGrid);

  void render();
  void setPosition(vec3 position);
  void rotateAlongAxis();
  bool checkOutOfGrid();
  void checkWin();
  void fall();
  void turnX(bool negDirection);
  void turnZ(bool negDirection);
  void checkAndToggleSwitch();
  bool checkOnFragile();

  GLfloat cutPrecision(GLfloat num);

};


#endif
