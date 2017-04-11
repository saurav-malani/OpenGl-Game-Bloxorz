#include "config.hpp"

#include "3DObject.hpp"
#include "Camera.hpp"
#include "shader.hpp"

#ifndef GRID
#define GRID

class Grid {

public:
  Model *darkTile, *lightTile, *switchTile, *winTile;
  vector<bool> tileRotated;
  Model *grid, *switches;

  Mesh *rotatingTile;
  GLint tileIndex, tilex, tilez;
  GLfloat tileRotationAngle;
  bool tileRotating, tileFalling;

  vector<vector<string> > gridDef;
  vec3 playerStartPosition, camTarget;
  Shader *shader;

  Grid(GLuint level, Shader *shader);

  void render();
  void level1Init();
  void level2Init();
  void modelsInit();

  void setTileInitRotation(Mesh *tile, GLuint index);
  void toggleSwitch(GLint x, GLint z);
  void moveTile(vec3 trans);
  void rotateTile(GLint mulFac);
  vec3 getStartPosition();
  vec3 getCamTarget();


};

#endif
