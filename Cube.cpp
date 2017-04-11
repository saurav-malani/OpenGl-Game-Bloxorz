#include "config.hpp"

#include "globalVars.hpp"

Cube::Cube(Shader *shader, GLchar *path, Grid *onGrid)
{
  this->shader = shader;
  this->cube = new Model(path, this->shader);

  this->width = 2.0f;
  this->height = 4.0f;
  this->depth = 2.0f;

  this->moves = 0;

  this->grid = onGrid;
  this->centerPosition = vec3(0.0f, 0.0f, 0.0f);
  this->instCenterPosition = vec3(0.0f, 0.0f, 0.0f);
  this->bottomCenterPosition = vec3(0.0f, -1.0f, 0.0f);
  this->topCenterPosition = vec3(0.0f, 1.0f, 0.0f);

  this->falling = false;
  this->turning = false;
  this->turnRotation = 0.0f;
}

void Cube::render()
{
  if(this->turning == true)
    this->rotateAlongAxis();

  if(this->falling == true)
    this->fall();

  GLint onScreenLoc = glGetUniformLocation(this->shader->id, "onScreen");
  glUniform1i(onScreenLoc, false);

  this->cube->render();
}

void Cube::setPosition(vec3 position)
{
  this->centerPosition = position;
  this->instCenterPosition = position;
  this->topCenterPosition = vec3(0.0f, 1.0f, 0.0f);
  this->bottomCenterPosition = vec3(0.0f, -1.0f, 0.0f);

  this->cube->moveTo(position);
}

void Cube::rotateAlongAxis()
{
  GLfloat incAngle = this->mulFac * 9.0f;

  this->cube->moveTo(-this->centerPosition);
  this->cube->moveTo(this->transform);

  this->cube->rotateAround(this->rotationAxis, incAngle);
  this->turnRotation += incAngle;

  this->cube->moveTo(this->invTransform);
  this->cube->moveTo(this->centerPosition);

  this->instCenterPosition = vec3(this->cube->modelMatrix * vec4(0.0f, 0.0f, 0.0f, 1.0f));

  if(fabs(this->turnRotation) >= 90.0f)
  {
    this->turning = false;

    this->turnRotation = 0.0f;

    this->centerPosition = vec3(this->cube->modelMatrix * vec4(0.0f, 0.0f, 0.0f, 1.0f));
    vec3 top = vec3(this->cube->modelMatrix * vec4(0.0f, 1.0f, 0.0f, 1.0f));
    vec3 bottom = vec3(this->cube->modelMatrix * vec4(0.0f, -1.0f, 0.0f, 1.0f));

    this->topCenterPosition = vec3(cutPrecision(top.x), cutPrecision(top.y), cutPrecision(top.z));
    this->bottomCenterPosition = vec3(cutPrecision(bottom.x), cutPrecision(bottom.y), cutPrecision(bottom.z));

    if(this->checkOutOfGrid() == true)
    {
      this->falling = true;
      gc->currentScene->score->lost = true;
    }

    if(this->grid->tileRotating == false)
      this->checkAndToggleSwitch();

    if(this->falling == false && this->checkOnFragile() == true)
      this->falling = true;

    if(this->falling == false)
      this->checkWin();

  }
}

void Cube::checkWin()
{
  GLint topx = floor(this->topCenterPosition.x) / 2;
  GLint topz = floor(this->topCenterPosition.z) / 2;

  GLint bottomx = floor(this->bottomCenterPosition.x) / 2;
  GLint bottomz = floor(this->bottomCenterPosition.z) / 2;

  GLint xLim = this->grid->gridDef.size() - 1;
  GLint zLim = this->grid->gridDef[0].size() - 1;

  bool expression1 = topx < 0 || topz < 0 || topx > xLim || topz > zLim;
  bool expression2 = bottomx < 0 || bottomz < 0 || bottomx > xLim || bottomz > zLim;

  if((expression1 && expression2) == false)
  {
    istringstream iss1(this->grid->gridDef[topx][topz]);
    vector<string> tokens{istream_iterator<string>(iss1), istream_iterator<string>()};
    if(topx == bottomx && topz == bottomz && tokens[0] == "win")
    {
      this->falling = true;
      gc->currentScene->score->won = true;
    }
  }
}

bool Cube::checkOnFragile()
{
  GLint topx = floor(this->topCenterPosition.x) / 2;
  GLint topz = floor(this->topCenterPosition.z) / 2;

  GLint bottomx = floor(this->bottomCenterPosition.x) / 2;
  GLint bottomz = floor(this->bottomCenterPosition.z) / 2;

  GLint xLim = this->grid->gridDef.size() - 1;
  GLint zLim = this->grid->gridDef[0].size() - 1;

  bool expression1 = topx < 0 || topz < 0 || topx > xLim || topz > zLim;
  bool expression2 = bottomx < 0 || bottomz < 0 || bottomx > xLim || bottomz > zLim;

  if((expression1 && expression2) == false)
  {
    istringstream iss(this->grid->gridDef[topx][topz]);
    vector<string> tokens{istream_iterator<string>(iss), istream_iterator<string>()};

    if(topx == bottomx && topz == bottomz && tokens[0] == "light")
    {
      this->grid->tilex = topx;
      this->grid->tilez = topz;
      this->grid->tileFalling = true;
      gc->currentScene->score->lost = true;

      return true;
    }
  }

  return false;
}

void Cube::checkAndToggleSwitch()
{
  GLint topx = floor(this->topCenterPosition.x) / 2;
  GLint topz = floor(this->topCenterPosition.z) / 2;

  GLint bottomx = floor(this->bottomCenterPosition.x) / 2;
  GLint bottomz = floor(this->bottomCenterPosition.z) / 2;

  GLint xLim = this->grid->gridDef.size() - 1;
  GLint zLim = this->grid->gridDef[0].size() - 1;

  bool expression1 = topx < 0 || topz < 0 || topx > xLim || topz > zLim;
  bool expression2 = bottomx < 0 || bottomz < 0 || bottomx > xLim || bottomz > zLim;

  if(expression1 == false)
  {
    istringstream iss1(this->grid->gridDef[topx][topz]);
    vector<string> topTokens{istream_iterator<string>(iss1), istream_iterator<string>()};
    if(topTokens[0] == "switch")
    {
      this->grid->toggleSwitch(topx, topz);
    }
  }

  if(expression2 == false)
  {
    istringstream iss2(this->grid->gridDef[bottomx][bottomz]);
    vector<string> bottomTokens{istream_iterator<string>(iss2), istream_iterator<string>()};

    if(bottomTokens[0] == "switch")
      this->grid->toggleSwitch(bottomx, bottomz);
  }
}

bool Cube::checkOutOfGrid()
{
  GLint topx = floor(this->topCenterPosition.x) / 2;
  GLint topz = floor(this->topCenterPosition.z) / 2;

  GLint bottomx = floor(this->bottomCenterPosition.x) / 2;
  GLint bottomz = floor(this->bottomCenterPosition.z) / 2;

  GLint xLim = this->grid->gridDef.size() - 1;
  GLint zLim = this->grid->gridDef[0].size() - 1;

  bool expression1 = topx < 0 || topz < 0 || topx > xLim || topz > zLim || this->grid->gridDef[topx][topz] == "nil";
  bool expression2 = bottomx < 0 || bottomz < 0 || bottomx > xLim || bottomz > zLim || this->grid->gridDef[bottomx][bottomz] == "nil";

  if((expression1 && expression2) == true)
    return true;
  else
    return false;
}

void Cube::fall()
{
  this->cube->moveTo(vec3(0.0f, -0.25f, 0.0f));
  this->centerPosition = this->centerPosition + vec3(0.0f, -0.25f, 0.0f);
}

void Cube::turnX(bool negDirection)
{
  if(this->turning == false)
  {
    this->moves++;
    this->mulFac = negDirection ? 1 : -1;
    this->turning = true;

    this->rotationAxis = vec3(0.0f, 0.0f, 1.0f);
    this->transform = vec3(this->mulFac * this->width / 2.0f, this->height / 2.0f, 0.0f);
    this->invTransform = -this->transform;

    GLfloat temp = this->width;
    this->width = this->height;
    this->height = temp;
  }
}

void Cube::turnZ(bool negDirection)
{
  if(this->turning == false)
  {
    this->moves++;
    this->mulFac = negDirection ? -1 : 1;
    this->turning = true;

    this->rotationAxis = vec3(1.0f, 0.0f, 0.0f);
    this->transform = vec3(0.0f, this->height / 2.0f, -this->mulFac * this->depth / 2.0f);
    this->invTransform = -this->transform;

    GLfloat temp = this->depth;
    this->depth = this->height;
    this->height = temp;
  }
}

GLfloat Cube::cutPrecision(GLfloat num)
{
  GLfloat one = trunc((num - trunc(num)) * 10);
  GLfloat two = trunc((num - trunc(num)) * 100);

  return((GLfloat) trunc(num) + one / 10.0f + two / 100.0f);
}
