#include <string>
#include <fstream>
#include <streambuf>

#include <iostream>
#include <stdexcept>

#include <sys/time.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "helper.h"
#include "scene.h"

int generateFrameVBO(GLuint shaderProgram) {
  GLfloat vertices[] = {
    -1, -1,  0, 0,
    -1, 1,   0, 1,
    1, 1,    1, 1,

    1, 1,    1, 1,
    1, -1,   1, 0,
    -1, -1,  0, 0,
  };

  GLuint vertexStride = sizeof(GLfloat) * 4;
  void *positionOffset = 0;
  void *coordOffset = (void *) (2 * sizeof(GLfloat));

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

GLuint generateFramebuffer() {
  GLuint fbo;
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  checkErrors();

  // Create a texture for use by this framebuffer
  GLuint fboTexture;
  glActiveTexture(nextTextureIndex());
  glGenTextures(1, &fboTexture);
  glBindTexture(GL_TEXTURE_2D, fboTexture);
  checkErrors();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL /*no data*/);
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

int main (int argv, char *argc[]) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_Window *window = SDL_CreateWindow("OpenGL", 100, 100, 640, 480, SDL_WINDOW_OPENGL);
  SDL_GLContext context = SDL_GL_CreateContext(window);

  glewExperimental = GL_TRUE; // necessary for modern opengl calls
  glewInit();
  checkErrors();

  CubeScene *scene = new CubeScene();

  // Generate vertex array object
  GLuint frameVAO;
  glGenVertexArrays(1, &frameVAO);
  glBindVertexArray(frameVAO);
  checkErrors();

  // We sometimes don't want to render directly to the screen. Let's render to a frame buffer
  // instead.
  GLuint fbo = generateFramebuffer();

  // Generate shader for rendering buffer
  GLuint renderBufferProgram = generateShaderProgram("render_buffer.vert", "render_buffer.frag");
  glUseProgram(renderBufferProgram);
  checkErrors();
  
  // Because fragment shaders can often write to multiple buffers, we need to specify which buffer
  // to bind the user-defined varying 'out' variable to.
  glBindFragDataLocation(renderBufferProgram, 0, "outFragColor");

  // Setup vbo for the quad for drawing the fbo
  GLuint frameVBO = generateFrameVBO(renderBufferProgram);
  int frameVBOElements = 6;

  struct timeval t;
  gettimeofday(&t, NULL);
  long int startTime = t.tv_sec * 1000 + t.tv_usec / 1000;

  SDL_Event windowEvent;
  while (true) {
    if (SDL_PollEvent(&windowEvent)) {
      if (windowEvent.type == SDL_QUIT) break;
      if (windowEvent.type == SDL_KEYUP) {
	if (windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
      }
    }

    gettimeofday(&t, NULL);
    long int currentTime = t.tv_sec * 1000 + t.tv_usec / 1000;
    float time = (float) (currentTime - startTime) / 1000.0f;
    
    ///////////////////////////////////////////////////////////////////////////////////////
    // Draw the 3d scene
    // Let's render to the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /*glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(renderBufferProgram);

      // Disable tests
      glDisable(GL_DEPTH_TEST);

      // Render the fbo quad
      glBindBuffer(GL_ARRAY_BUFFER, frameVBO);
      glDrawArrays(GL_TRIANGLES, 0, frameVBOElements);*/

    // Double check that our rendering worked
    // GLfloat data[4];
    // glReadPixels(0,0,1,1, GL_RGBA, GL_FLOAT, &data);
    // printf("%f, %f, %f, %f\n", data[0], data[1], data[2], data[3]);

    // glDrawElements(GL_TRIANGLES, numElements /*num*/, GL_UNSIGNED_INT, 0 /*offset*/);

    scene->Render(time);

    SDL_GL_SwapWindow(window);
    checkErrors();
  }

  SDL_GL_DeleteContext(context);
  SDL_Quit();
  return 0;
}
