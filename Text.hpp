#include "config.hpp"

#include "shader.hpp"
#include "3DObject.hpp"

#ifndef TEXT
#define TEXT


class Text {

public:

  Model *units, *tens, *hundreds, *lostModel, *wonModel;
  Shader *shader;
  GLint val;

  bool lost, won;

  GLint unitsIndex, tensIndex, hundredsIndex;

  Text(Shader *shader, GLchar *path);
  void render();
  void setVal(GLint val);

};


#endif
