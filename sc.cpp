#include "config.hpp"

#include "globalVars.hpp"

Scene::Scene(int backgroundColor[], int level)
{
  glEnable(GL_DEPTH_TEST);

  this->red = backgroundColor[0] / 255.0f;
  this->green = backgroundColor[1] / 255.0f;
  this->blue = backgroundColor[2] / 255.0f;

  this->textureShader = new Shader("Shaders/VertexTexture", "Shaders/FragmentTexture");
  this->textureShader->use();

  this->defaultCamera = new Camera();
  this->defaultCamera->look(vec3(15.0f, 15.0f, 35.0f), vec3(15.0f, 0.0f, 6.0f), vec3(0.0f, 1.0f, 0.0f));
  this->currentCameraMode = "helicopter";

  this->score = new Text(this->textureShader, "Models/number.obj");

  this->setupLevel(level);
}

void Scene::changeCameraMode(string mode)
{
  this->currentCameraMode = mode;

  if(mode == "top")
  {
    this->defaultCamera->look(vec3(15.0f, 30.0f, 6.0f), vec3(15.0f, 0.0f, 6.0f), vec3(1.0f, 0.0f, 0.0f));
  }
  else if(mode == "helicopter" || mode == "top")
  {
    this->defaultCamera->thetax = 90.0f;
    this->defaultCamera->look(vec3(15.0f, 15.0f, 35.0f), vec3(15.0f, 0.0f, 6.0f), vec3(0.0f, 1.0f, 0.0f));
  }

}

void Scene::render()
{
  this->paintBackgroud();
  this->renderViewport();

  this->grid->render();
  this->player->render();

  this->score->setVal(this->player->moves);
  this->score->render();
}

void Scene::renderViewport()
{
  GLint shaderId = this->textureShader->id;

  if(this->currentCameraMode == "follow")
    this->defaultCamera->look(this->player->instCenterPosition + vec3(-10.0f, 5.0f, 0.0f), this->player->instCenterPosition, vec3(0.0f, 1.0f, 0.0f));

  GLint viewLoc = glGetUniformLocation(shaderId, "view");
  GLint projLoc = glGetUniformLocation(shaderId, "projection");

  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(this->defaultCamera->getViewMatrix()));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(this->defaultCamera->getProjectionMatrix()));
}

void Scene::setupLevel(GLuint level)
{
  vec3 position, camTarget;

  if(level == 1)
  {
    this->grid = new Grid(level, this->textureShader);
    this->player = new Cube(this->textureShader, "Models/cuboid.obj", this->grid);

    position = this->grid->getStartPosition();
    camTarget = this->grid->getCamTarget();
  }
  else if(level == 2)
  {
    this->grid = new Grid(level, this->textureShader);
    this->player = new Cube(this->textureShader, "Models/cuboid.obj", this->grid);

    position = this->grid->getStartPosition();
    camTarget = this->grid->getCamTarget();
  }

  this->player->setPosition(position);
  this->defaultCamera->look(vec3(15.0f, 15.0f, 35.0f), camTarget, vec3(0.0f, 1.0f, 0.0f));
}

void Scene::paintBackgroud()
{
  glClearColor(this->red, this->green, this->blue, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
