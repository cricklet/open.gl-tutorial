#include "scene.h"
#include "helper.h"

static void
setupTexture(const GLuint shaderProgram, GLuint &texKitten, GLuint &texPuppy) {
  // We can store 2d textures in a texture buffer
  int texKittenIndex = 0;
  texKitten = loadTexture("kitten.png", texKittenIndex);

  int texPuppyIndex = 1;
  texPuppy = loadTexture("puppy.png", texPuppyIndex);

  GLint texKittenAttrib = glGetUniformLocation(shaderProgram, "texKitten");
  glUniform1i(texKittenAttrib, texKittenIndex);

  GLint texPuppyAttrib = glGetUniformLocation(shaderProgram, "texPuppy");
  glUniform1i(texPuppyAttrib, texPuppyIndex);

  checkErrors();
}

CubeScene::CubeScene () {
  
  setupTexture(shaderProgram, texKitten, texPuppy);
 }

CubeScene::~CubeScene () {
  
}
