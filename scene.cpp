#include "scene.h"

#include <SOIL/soil.h>

static GLuint
loadTexture(const char *filename, int index) {
  GLuint tex;
  glGenTextures(1, &tex);
  glActiveTexture(GL_TEXTURE0 + (GLuint) index);
  glBindTexture(GL_TEXTURE_2D, tex);

  int width, height;
  unsigned char* image =
    SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

  glTexImage2D(GL_TEXTURE_2D, 0 /*mipmap*/, GL_RGB, width, height, 0, GL_RGB,
	       GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
}

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
}

CubeScene::CubeScene () {
  
}

CubeScene::~CubeScene () {
  
}
