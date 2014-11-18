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

int generateSceneVBO(GLuint shaderProgram) {
  GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //xyz rgb uv
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
  };

  GLuint vertexStride = sizeof(GLfloat) * 8;
  void *positionOffset = 0;
  void *colorOffset = (void *) (3 * sizeof(GLfloat));
  void *texOffset   = (void *) (6 * sizeof(GLfloat));
  
  // We don't want to have to copy vertices to the GPU every time we render. Instead, we can copy
  // the vertex information into a vertex buffer object.
  GLuint vbo;
  glGenBuffers(1, &vbo); // generate one buffer object name
  glBindBuffer(GL_ARRAY_BUFFER, // target to bind to
	       vbo);
  glBufferData(GL_ARRAY_BUFFER, // target buffer object
	       sizeof(vertices),
	       vertices,
	       GL_STATIC_DRAW); // vs GL_DYNAMIC_DRAW vs GL_STREAM_DRAW
  checkErrors();

  // Shader needs to know how to get the input attributes.
  GLint posAttrib = glGetAttribLocation(shaderProgram, "inVertPosition");
  glVertexAttribPointer(posAttrib,
			3,  // size of vector i.e. vec2
			GL_FLOAT,
			GL_FALSE, // should be normalized
			vertexStride,  // stride: # bytes between each attribute in the array
			positionOffset); // offset: # bytes from the start of the array
  glEnableVertexAttribArray(posAttrib);
  checkErrors();

  GLint colorAttrib = glGetAttribLocation(shaderProgram, "inVertColor");
  glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE,
			vertexStride,
			colorOffset);
  glEnableVertexAttribArray(colorAttrib);
  checkErrors();

  GLint texCoordAttrib = glGetAttribLocation(shaderProgram, "inVertTexCoord");
  glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE,
			vertexStride,
			texOffset);
  glEnableVertexAttribArray(texCoordAttrib);
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

GLuint generateShaderProgram(const char *vertSource, const char *fragSource) {
  // Load the shaders from the filesystem.
  GLuint vertShader = compileShader(vertSource, GL_VERTEX_SHADER);
  GLuint fragShader = compileShader(fragSource, GL_FRAGMENT_SHADER);
  checkErrors();

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertShader);
  glAttachShader(shaderProgram, fragShader);
  checkErrors();

  // Because fragment shaders can often write to multiple buffers, we need to specify which buffer
  // to bind the user-defined varying 'out' variable to.
  glBindFragDataLocation(shaderProgram,
			 0, // color number (buffer) to bind to
			 "outFragColor");
  checkErrors();

  glLinkProgram(shaderProgram);
  checkErrors();
  
  return shaderProgram;
}

void setupTextures(GLuint shaderProgram) {
  // We can store 2d textures in a texture buffer
  int texKittenIndex = 0;
  GLuint texKitten = loadTexture("kitten.png", texKittenIndex);

  int texPuppyIndex = 1;
  GLuint texPuppy = loadTexture("puppy.png", texPuppyIndex);

  GLint texKittenAttrib = glGetUniformLocation(shaderProgram, "texKitten");
  glUniform1i(texKittenAttrib, texKittenIndex);

  GLint texPuppyAttrib = glGetUniformLocation(shaderProgram, "texPuppy");
  glUniform1i(texPuppyAttrib, texPuppyIndex);
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

  /* GLuint elements[] = {
    0, 1, 2, 3, 0, 2
    }; */

  // We don't want to have to call glVertexAttribPointer to reset the inputs every time we enable
  // a shader (glUseProgram). Instead, we can store all the state needed to use a shader inside
  // a vertex array object.
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  checkErrors();

  // We sometimes don't want to render directly to the screen. Let's render to a frame buffer
  // instead.
  GLuint fbo = generateFramebuffer();

  // We want to be able to render the vertices in any order (with repetition). To do this, we
  // create an element array buffer object.
  /*GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
  checkErrors();*/

  // Generate shader for rendering buffer
  GLuint renderBufferProgram = generateShaderProgram("render_buffer.vert", "render_buffer.frag");
  glUseProgram(renderBufferProgram);
  checkErrors();
  
  // Generate shader for rendering cube
  GLuint renderSceneProgram = generateShaderProgram("render_scene.vert", "render_scene.frag");
  glUseProgram(renderSceneProgram);
  checkErrors();

  // Get shader program varying uniforms
  GLint overrideColor = glGetUniformLocation(renderSceneProgram, "overrideColor");
  GLint timeUniform = glGetUniformLocation(renderSceneProgram, "time");
  GLint modelTransUniform = glGetUniformLocation(renderSceneProgram, "inVertModelTrans");
  GLint viewTransUniform  = glGetUniformLocation(renderSceneProgram, "inVertViewTrans");
  GLint projTransUniform  = glGetUniformLocation(renderSceneProgram, "inVertProjTrans");
  checkErrors();

  // Setup vertex buffer object for cube.
  // Must happen after glUseProgram because it sets up attrib locations.
  GLuint sceneVBO = generateSceneVBO(renderSceneProgram);
  int cubeStart = 0;
  int cubeElements = 36;
  int floorStart = 36;
  int floorElements = 6;
  checkErrors();

  // Setup vbo for the quad for drawing the fbo
  GLuint frameVBO = generateFrameVBO(renderBufferProgram);
  int frameVBOElements = 6;

  // Setup textures
  setupTextures(renderSceneProgram);

  glm::mat4 viewTrans;
  viewTrans = glm::lookAt(
    glm::vec3(3.0f, 1.0f, 1.0f), // location of camera
    glm::vec3(0,0,0), // direction of camera
    glm::vec3(0,0,1)  // camera up vector
  );

  glm::mat4 projTrans;
  projTrans = glm::perspective(
    45.0f, // fov y
    800.0f / 600.0f, // aspect
    0.2f,  // near
    10.0f  //far
  );
  glUniformMatrix4fv(viewTransUniform,  1, GL_FALSE, glm::value_ptr(viewTrans));
  glUniformMatrix4fv(projTransUniform,  1, GL_FALSE, glm::value_ptr(projTrans));

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
    glBindBuffer(GL_ARRAY_BUFFER, sceneVBO);
    glUseProgram(renderSceneProgram);

      // Use depth test
      glEnable(GL_DEPTH_TEST);

      // Clear the screen to black
      glClearColor(1, 1, 1, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      // Vary the time uniform
      glUniform1f(timeUniform, time);

      // Setup model transform
      glm::mat4 modelTrans;
      modelTrans = glm::rotate(modelTrans, time, glm::vec3(0,0,1));
      glUniformMatrix4fv(modelTransUniform, 1, GL_FALSE, glm::value_ptr(modelTrans));

      // Draw cube
      glDrawArrays(GL_TRIANGLES, cubeStart, cubeElements);

      // Time for fancy stencil rendering
      glEnable(GL_STENCIL_TEST);

	// Draw floor
	glStencilFunc(GL_ALWAYS, 1, 0xFF); // set stencil values to 1 where rendered
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glDepthMask(GL_FALSE); // don't write to depth buffer
	glStencilMask(0xFF); // write to stencil buffer

	glClear(GL_STENCIL_BUFFER_BIT); // clear the stencil

	glDrawArrays(GL_TRIANGLES, floorStart, floorElements);

	// Get ready to draw reflection
	glStencilFunc(GL_EQUAL, 1, 0xFF); // pass test if stencil value is 1
	glDepthMask(GL_TRUE); // reenable depth
	glStencilMask(0x00); // redisable stencil

	// Flip model transform (reflection)
	modelTrans = glm::scale(
	  glm::translate(modelTrans, glm::vec3(0,0,-1)),
	  glm::vec3(1,1,-1)
	);
	glUniformMatrix4fv(modelTransUniform, 1, GL_FALSE, glm::value_ptr(modelTrans));

	// Draw reflection
	glUniform3f(overrideColor, 0.3f, 0.3f, 0.3f);
	glDrawArrays(GL_TRIANGLES, cubeStart, cubeElements);
	glUniform3f(overrideColor, 1.0f, 1.0f, 1.0f);

      // We're done with the stencil rendering
      glDisable(GL_STENCIL_TEST);

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

    SDL_GL_SwapWindow(window);
    checkErrors();
  }

  // glDeleteTextures(1, &texKitten);
  // glDeleteTextures(1, &texPuppy);

  glDeleteProgram(renderSceneProgram);
  // glDeleteShader(fragShader);
  // glDeleteShader(vertShader);

  //glDeleteBuffers(1, &ebo);
  glDeleteBuffers(1, &sceneVBO);

  glDeleteVertexArrays(1, &vao);

  SDL_GL_DeleteContext(context);
  SDL_Quit();
  return 0;
}
