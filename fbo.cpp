#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>

#include "fbo.h"
#include "helper.h"

static GLfloat vertices[] = {
  -1, -1,  0, 0,
  -1, 1,   0, 1,
  1, 1,    1, 1,

  1, 1,    1, 1,
  1, -1,   1, 0,
  -1, -1,  0, 0,
};


static GLuint vertexStride = sizeof(GLfloat) * 4;
static void *positionOffset = 0;
static void *coordOffset = (void *) (2 * sizeof(GLfloat));

static 
GLuint generateVBO(GLuint shaderProgram) {
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  checkErrors();

  GLint posAttrib = glGetAttribLocation(shaderProgram, "inVertPosition");
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, vertexStride, positionOffset);
  glEnableVertexAttribArray(posAttrib);
  checkErrors();

  GLint coordAttrib = glGetAttribLocation(shaderProgram, "inVertBufferCoord");
  glVertexAttribPointer(coordAttrib, 2, GL_FLOAT, GL_FALSE, vertexStride, coordOffset);
  glEnableVertexAttribArray(coordAttrib);
  checkErrors();
  
  return vbo;
}

static 
GLuint generateFBO(int textureIndex) {
  GLuint fbo;
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  checkErrors();

  // Create a texture for use by this framebuffer
  GLuint fboTexture;
  glActiveTexture(GL_TEXTURE0 + textureIndex);
  glGenTextures(1, &fboTexture);
  glBindTexture(GL_TEXTURE_2D, fboTexture);
  checkErrors();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL /*no data*/);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  checkErrors();
  
  // Bind the texture to the frambuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);
  checkErrors();

  // Create a render buffer object for use by this framebuffer
  GLuint fboRenderBuffer;
  glGenRenderbuffers(1, &fboRenderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, fboRenderBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, 
			    GL_RENDERBUFFER, fboRenderBuffer);
  checkErrors();

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "glCheckFramebufferStatus() failed\n";
  }

  // Return to rendering to the default framebuffer (the screen)
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return fbo;
}

FBO::FBO() {
  glGenVertexArrays(1, &this->vao);
  glBindVertexArray(this->vao);
  checkErrors();
  
  GLuint textureIndex = nextTextureIndex();
  this->fbo = generateFBO(textureIndex);

  this->shaderProgram = generateShaderProgram("render_buffer.vert", "render_buffer.frag");
  glUseProgram(this->shaderProgram);
  
  glBindFragDataLocation(this->shaderProgram, 0, "outFragColor");

  GLint texAttrib = glGetUniformLocation(this->shaderProgram, "buffer");
  glUniform1i(texAttrib, textureIndex);
  checkErrors();

  this->vbo = generateVBO(this->shaderProgram);
}

FBO::~FBO() {
}

void FBO::Bind() {
}

void FBO::Render() {
}
