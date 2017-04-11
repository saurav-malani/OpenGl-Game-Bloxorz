#include "config.hpp"

#include "globalVars.hpp"

Grid::Grid(GLuint level, Shader *shader)
{
  this->shader = shader;
  this->darkTile = new Model("Models/dark_tile.obj", this->shader);
  this->lightTile = new Model("Models/light_tile.obj", this->shader);
  this->switchTile = new Model("Models/switch_tile.obj", this->shader);
  this->winTile = new Model("Models/win_well.obj", this->shader);

  this->rotatingTile = NULL;
  this->tileIndex = -1;
  this->tileRotating = false;
  this->tileFalling = false;
  this->tileRotationAngle = 0.0f;
  this->tilex = -1;
  this->tilez = -1;

  if(level == 1)
    this->level1Init();
  else if(level == 2)
    this->level2Init();
}

vec3 Grid::getStartPosition()
{
  return this->playerStartPosition;
}

vec3 Grid::getCamTarget()
{
  return this->camTarget;
}

void Grid::toggleSwitch(GLint x, GLint z)
{
  istringstream iss(this->gridDef[x][z]);
  vector<string> tokens{istream_iterator<string>(iss), istream_iterator<string>()};

  GLint fx, fz;
  cout << tokens[1] << " " << tokens[2] << endl;
  fx = atoi(tokens[1].c_str());
  fz = atoi(tokens[2].c_str());

  GLint index = this->gridDef[0].size() * fx + fz;

  this->tileIndex = index;
  this->rotatingTile = this->grid->meshes[index];
  this->tileRotating = true;
  this->tilex = fx;
  this->tilez = fz;
}

void Grid::level1Init()
{
  this->playerStartPosition = vec3(2.0f, 2.0f, 8.0f);
  this->camTarget = vec3(15.0f, 0.0f, 6.0f);

  this->gridDef = {
    {"nil", "dark", "light", "light", "dark", "light"},

    {"nil", "dark", "light", "dark", "light", "dark"},

    {"nil", "dark", "dark", "light", "switch 4 1", "dark"},

    {"nil", "dark", "light", "dark", "dark", "dark"},

    {"nil", "light r", "nil", "nil", "nil", "nil"},

    {"nil", "light", "nil", "nil", "nil", "nil"},

    {"dark", "dark", "light", "dark", "dark", "dark"},

    {"dark", "dark", "light", "dark", "light", "dark"},

    {"dark", "light", "light", "dark", "switch 10 1", "dark"},

    {"dark", "dark", "dark", "light", "dark", "dark"},

    {"nil", "light r", "nil", "nil", "nil", "nil"},

    {"nil", "light", "nil", "nil", "nil", "nil"},

    {"dark", "dark", "dark", "dark", "dark", "nil"},

    {"light", "light", "win", "dark", "light", "nil"},

    {"dark", "dark", "light", "dark", "light", "nil"},
  };

  this->modelsInit();
}

void Grid::level2Init()
{
  this->playerStartPosition = vec3(0.0f, 2.0f, 18.0f);
  this->camTarget = vec3(15.0f, 0.0f, 10.0f);

  this->gridDef = {
    {"nil", "nil", "nil", "nil", "nil", "nil", "nil", "light", "light", "dark", "nil"},

    {"nil", "nil", "nil", "nil", "nil", "nil", "nil", "light", "light", "light", "light"},

    {"nil", "nil", "nil", "nil", "nil", "dark", "dark", "light", "light", "light", "dark"},

    {"dark", "dark", "dark", "nil", "nil", "nil", "light", "light", "dark", "light", "light"},

    {"dark", "win", "dark", "nil", "nil", "nil", "nil", "nil", "light", "light", "light"},

    {"dark", "dark", "dark", "nil", "nil", "nil", "nil", "nil", "dark", "light", "light"},

    {"dark", "dark", "nil", "nil", "nil", "nil", "nil", "nil", "light", "light", "nil"},

    {"dark", "nil", "nil", "nil", "light", "light", "light", "dark", "light", "light", "nil"},

    {"light", "nil", "nil", "nil", "dark", "light", "light", "dark", "dark", "light", "light"},

    {"light", "nil", "nil", "nil", "dark", "nil", "nil", "dark", "light", "light", "light"},

    {"dark", "nil", "nil", "nil", "nil", "nil", "nil", "nil", "nil", "light", "light"},

    {"dark", "nil", "nil", "nil", "nil", "nil", "nil", "nil", "nil", "light", "light"},

    {"dark", "light", "light", "dark", "nil", "nil", "nil", "dark", "switch", "dark", "dark"},

    {"dark", "dark", "dark", "dark", "nil", "nil", "nil", "dark", "dark", "nil", "nil"},

    {"nil", "nil", "nil", "dark", "dark", "dark", "dark", "dark", "nil", "nil", "nil"},
  };

  this->modelsInit();
}

void Grid::modelsInit()
{
  Model *newModel = new Model(this->shader);
  Model *switchNew = new Model(this->shader);

  for(GLuint i=0; i < this->gridDef.size(); i++)
  {
    vector<string> current = this->gridDef[i];
    for(GLuint j=0; j < current.size(); j++)
    {
      this->tileRotated.push_back(false);

      istringstream iss(this->gridDef[i][j]);
      vector<string> tokens{istream_iterator<string>(iss), istream_iterator<string>()};

      if(tokens[0] == "dark")
      {
        Mesh *newMesh = newModel->cloneMesh(this->darkTile->meshes[0], vec3(i * 2.0f, 0.0f, j * 2.0f));
        if(tokens.size() > 1 && tokens[1] == "r")
        {
          this->gridDef[i][j] = "nil";
          this->setTileInitRotation(newMesh, this->gridDef[0].size() * i + j);
        }
      }
      else if(tokens[0] == "light")
      {
        Mesh *newMesh = newModel->cloneMesh(this->lightTile->meshes[0], vec3(i * 2.0f, 0.0f, j * 2.0f));
        if(tokens.size() > 1 && tokens[1] == "r")
        {
          this->gridDef[i][j] = "nil";
          this->setTileInitRotation(newMesh, this->gridDef[0].size() * i + j);
        }
      }
      else if(tokens[0] == "switch")
      {
        newModel->cloneMesh(this->darkTile->meshes[0], vec3(i * 2.0f, 0.0f, j * 2.0f));
        switchNew->cloneMesh(this->switchTile->meshes[0], vec3(i * 2.0f, 0.0f, j * 2.0f));
      }
      else if(tokens[0] == "win")
      {
        newModel->cloneMesh(this->winTile->meshes[0], vec3(i * 2.0f, 0.0f, j * 2.0f));
      }
      else if(tokens[0] == "nil")
      {
        Mesh *dummyMesh = new Mesh();
        newModel->meshes.push_back(dummyMesh);
      }
    }
  }

  this->switches = switchNew;
  this->grid = newModel;
}

void Grid::setTileInitRotation(Mesh *tile, GLuint index)
{
  vec3 tileTransform = vec3(-1.0f, 0.2f, 0.0f);

  tile->moveTo(-tile->position);

  tile->moveTo(tileTransform);
  tile->rotateAround(vec3(0.0f, 0.0f, 1.0f), 180.0f);
  tile->moveTo(-tileTransform);

  tile->moveTo(tile->position);
  tile->position = vec3(tile->modelMatrix * vec4(0.0f, 0.0f, 0.0f, 1.0f));
  this->tileRotated[index] = true;
}

void Grid::moveTile(vec3 trans)
{
  Mesh *tile = this->grid->meshes[this->tilex * this->gridDef[0].size() + this->tilez];
  tile->moveTo(trans);
}

void Grid::rotateTile(GLint mulFac)
{
  GLfloat incAngle = mulFac * 5.0f;
  vec3 tileTransform = vec3(-mulFac * 1.0f, mulFac * 0.2f, 0.0f);

  this->rotatingTile->moveTo(-this->rotatingTile->position);

  this->rotatingTile->moveTo(tileTransform);
  this->rotatingTile->rotateAround(vec3(0.0f, 0.0f, 1.0f), incAngle);
  this->rotatingTile->moveTo(-tileTransform);

  this->rotatingTile->moveTo(this->rotatingTile->position);

  this->tileRotationAngle += fabs(incAngle);

  if(this->tileRotationAngle >= 180.0f)
  {
    this->rotatingTile->position = vec3(this->rotatingTile->modelMatrix * vec4(0.0f, 0.0f, 0.0f, 1.0f));
    this->tileRotated[this->tileIndex] = !this->tileRotated[this->tileIndex];

    if(this->tileRotated[this->tileIndex] == true)
      this->gridDef[this->tilex][this->tilez] = "nil";
    else
      this->gridDef[this->tilex][this->tilez] = "dark";

    this->tileRotationAngle = 0.0f;
    this->tileRotating = false;
    this->rotatingTile = NULL;
    this->tileIndex = -1;
    this->tilex = -1;
    this->tilez = -1;
  }
}

void Grid::render()
{
  if(this->tileRotating == true)
  {
    GLint mulFac = this->tileRotated[this->tileIndex] ? -1 : 1;
    this->rotateTile(mulFac);
  }

  if(this->tileFalling == true)
    this->moveTile(vec3(0.0f, -1.0f, 0.0f));

  GLint onScreenLoc = glGetUniformLocation(this->shader->id, "onScreen");
  glUniform1i(onScreenLoc, false);

  this->switches->render();
  this->grid->render();
}
