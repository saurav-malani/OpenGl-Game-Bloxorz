#include "config.hpp"

#include "globalVars.hpp"

Model::Model(Shader *shader)
{
  this->shader = shader;
}

Model::Model(GLchar* path, Shader *shader)
{
  this->shader = shader;
  this->loadModel(path);
}

void Model::resetModelMatrix()
{
  mat4 newModelMatrix;
  this->modelMatrix = newModelMatrix;
}

void Model::moveTo(vec3 position)
{
  mat4 nMat;
  nMat = translate(nMat, position);
  this->modelMatrix = nMat * this->modelMatrix;
}

void Model::rotateAround(vec3 axis, GLfloat angle)
{
  mat4 nMat;
  nMat = rotate(nMat, radians(angle), axis);
  this->modelMatrix = nMat * this->modelMatrix;
}

void Model::render()
{
  for(int i=0;i < this->meshes.size();i++)
  {
    if(this->meshes[i]->vertices.size() != 0)
      this->meshes[i]->render(this->modelMatrix);
  }
}

Mesh* Model::cloneMesh(Mesh *mesh, vec3 position)
{
  Mesh *newMesh = new Mesh(this->shader, mesh);
  newMesh->moveTo(position);
  newMesh->position = position;

  this->meshes.push_back(newMesh);

  return newMesh;
}

void Model::loadModel(string path)
{
  Assimp::Importer assimp;
  const aiScene* scene = assimp.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    cout << "ERROR::ASSIMP::" << assimp.GetErrorString() << endl;
    wm->close();
  }

  this->scene = scene;
  this->genMeshes(scene->mRootNode);
}

void Model::genMeshes(aiNode* rootNode)
{
  for(GLuint i=0;i < scene->mNumMeshes; i++)
  {
    aiMesh* mesh = scene->mMeshes[i];
    this->meshes.push_back(this->genMeshObj(mesh));
  }
}

Mesh* Model::genMeshObj(aiMesh* mesh)
{
  vector<Vertex> vertices;
  vector<GLuint> indices;
  vector<Texture> textures;

  for(GLuint i = 0; i < mesh->mNumVertices; i++)
  {
    Vertex vertex;
    glm::vec3 vector;

    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.position = vector;

    vector.x = mesh->mNormals[i].x;
    vector.y = mesh->mNormals[i].y;
    vector.z = mesh->mNormals[i].z;
    vertex.normal = vector;

    if(mesh->mTextureCoords[0])
    {
        glm::vec2 vec;

        vec.x = mesh->mTextureCoords[0][i].x;
        vec.y = mesh->mTextureCoords[0][i].y;
        vertex.texCoords = vec;
    }
    else
        vertex.texCoords = glm::vec2(0.0f, 0.0f);
    vertices.push_back(vertex);
  }

  for(GLuint i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];

    for(GLuint j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
  }

  if(mesh->mMaterialIndex > 0)
  {
    vector<Texture> diffuseMaps;

    for(GLint i=mesh->mMaterialIndex;i>0;i--)
    {
      aiMaterial* material = scene->mMaterials[i];
      this->loadTextures(diffuseMaps, material, aiTextureType_DIFFUSE, "texture_diffuse");
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    }

    /*
    vector<Texture> specularMaps = this->loadTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    */
  }

  Mesh* retObj = new Mesh(this->shader, vertices, indices, textures, mesh->mName.C_Str());
  return retObj;
}

void Model::loadTextures(vector<Texture> &textures, aiMaterial* mat, aiTextureType type, string typeName)
{
  for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
  {
    aiString str;
    mat->GetTexture(type, i, &str);
    GLboolean skip = false;

    for(GLuint j = 0; j < textures_loaded.size(); j++)
    {
      if(textures_loaded[j].path == str)
      {
        textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }
    }
    if(!skip)
    {
      Texture texture;
      texture.id = this->getTexture(str.C_Str());
      texture.type = typeName;
      texture.path = str;
      textures.push_back(texture);
      this->textures_loaded.push_back(texture);
    }
  }
}

GLuint Model::getTexture(const char * filename)
{
  string file = string(filename);
  file.replace(0, 3, "");

  GLuint texId;
  GLint width, height;

  glGenTextures(1, &texId);
  unsigned char* imageName = SOIL_load_image(file.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

  glBindTexture(GL_TEXTURE_2D, texId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageName);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  SOIL_free_image_data(imageName);

  return texId;
}



Mesh::Mesh()
{
  //dummy constructor
}

Mesh::Mesh(Shader *shader, Mesh *mesh)
{
  this->vertices = mesh->vertices;
  this->indices = mesh->indices;
  this->textures = mesh->textures;
  this->shader = mesh->shader;
  this->position = vec3(0.0f, 0.0f, 0.0f);

  this->VAO = mesh->VAO;
}

Mesh::Mesh(Shader *shader, vector<Vertex> &vertices, vector<GLuint> indices, vector<Texture> textures, const GLchar  *name)
{
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;
  this->shader = shader;

  string temp(name);
  this->name = temp;

  this->setupMesh();
}

void Mesh::moveTo(vec3 position)
{
  mat4 newModelMatrix;
  newModelMatrix = translate(newModelMatrix, position);

  this->modelMatrix = newModelMatrix * this->modelMatrix;
}

void Mesh::rotateAround(vec3 axis, GLfloat angle)
{
  mat4 nMat;
  nMat = rotate(nMat, radians(angle), axis);
  this->modelMatrix = nMat * this->modelMatrix;
}

void Mesh::setupMesh()
{
  glGenVertexArrays(1, &this->VAO);
  glBindVertexArray(this->VAO);

  glGenBuffers(1, &this->VBO);
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &this->EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, texCoords));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::render(mat4 parentModelMatrix)
{
  GLuint diffuseTex = 1;
  GLuint specTex = 1;

  glBindVertexArray(this->VAO);

  for(GLuint i=0;i<this->textures.size();i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);

    stringstream ss;
    string name = this->textures[i].type, number;
    if(name == "texture_diffuse")
      ss << diffuseTex++;
    number = ss.str();

    GLint loc = glGetUniformLocation(shader->id, (name + number).c_str());
    glUniform1i(loc, i);
    glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
  }

  mat4 finalModelMatrix = parentModelMatrix * this->modelMatrix;
  GLint modelLoc = glGetUniformLocation(this->shader->id, "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(finalModelMatrix));

  glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
