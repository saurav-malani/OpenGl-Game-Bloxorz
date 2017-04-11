#include "config.hpp"

#include "shader.hpp"
#include "3DObject.hpp"
#include "Camera.hpp"
#include "Grid.hpp"
#include "Cube.hpp"
#include "Text.hpp"

#ifndef SCENE
#define SCENE

class Scene {

private:
  vector<Model> elements;
  GLfloat red, green, blue;
  Shader *textureShader;

  void renderViewport();
  void setupLevel(GLuint level);

public:
  Grid *grid;
  Camera *defaultCamera;
  string currentCameraMode;
  Cube* player;
  Text* score;

  Scene(int backgroundColor[], int level);
  void changeCameraMode(string mode);
  void paintBackgroud();
  void render();

};

#endif
