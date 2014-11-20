#include <sys/time.h>
#include "scene.h"
#include "helper.h"

static GLfloat vertices[] = {
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

static GLuint vertexStride = sizeof(GLfloat) * 8;
static void *positionOffset = 0;
static void *colorOffset = (void *) (3 * sizeof(GLfloat));
static void *texOffset   = (void *) (6 * sizeof(GLfloat));

static int cubeStart = 0;
static int cubeElements = 36;
static int floorStart = 36;
static int floorElements = 6;

static int generateVBO() {
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
}

static void bindVBOAttributes(GLuint shaderProgram) {
  // Shader needs to know how to get the input attributes.
  GLint posAttrib = glGetAttribLocation(shaderProgram, "inVertPosition");
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, vertexStride, positionOffset);
  glEnableVertexAttribArray(posAttrib);
  checkErrors();

  GLint colorAttrib = glGetAttribLocation(shaderProgram, "inVertColor");
  glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, vertexStride, colorOffset);
  glEnableVertexAttribArray(colorAttrib);
  checkErrors();

  GLint texCoordAttrib = glGetAttribLocation(shaderProgram, "inVertTexCoord");
  glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, vertexStride, texOffset);
  glEnableVertexAttribArray(texCoordAttrib);
  checkErrors();
}

CubeScene::CubeScene () {
  // Generate vertex array object
  glGenVertexArrays(1, &this->vao);
  glBindVertexArray(this->vao);
  checkErrors();
  
  // Generate shader for rendering cube
  this->shaderProgram = generateShaderProgram("render_scene.vert", "render_scene.frag");
  checkErrors();

  glBindFragDataLocation(this->shaderProgram, 0, "outFragColor");
  glUseProgram(this->shaderProgram);
  checkErrors();

  // Store the vertices
  this->vbo = generateVBO();
  checkErrors();

  bindVBOAttributes(this->shaderProgram);
  checkErrors();

  // Get shader program varying uniforms
  this->overrideColor = glGetUniformLocation(this->shaderProgram, "overrideColor");
  this->timeUniform = glGetUniformLocation(this->shaderProgram, "time");
  this->modelTransUniform = glGetUniformLocation(this->shaderProgram, "inVertModelTrans");
  this->viewTransUniform  = glGetUniformLocation(this->shaderProgram, "inVertViewTrans");
  this->projTransUniform  = glGetUniformLocation(this->shaderProgram, "inVertProjTrans");
  checkErrors();
  
  // We can store 2d textures in a texture buffer
  int texKittenIndex = 0;
  int texPuppyIndex = 1;

  this->texKitten = loadTexture("kitten.png", texKittenIndex);
  this->texPuppy = loadTexture("puppy.png", texPuppyIndex);

  GLint texKittenAttrib = glGetUniformLocation(this->shaderProgram, "texKitten");
  glUniform1i(texKittenAttrib, texKittenIndex);
  GLint texPuppyAttrib = glGetUniformLocation(this->shaderProgram, "texPuppy");
  glUniform1i(texPuppyAttrib, texPuppyIndex);
  checkErrors();

  this->viewTrans = glm::lookAt(
    glm::vec3(3.0f, 1.0f, 1.0f), // location of camera
    glm::vec3(0,0,0), // direction of camera
    glm::vec3(0,0,1)  // camera up vector
  );

  this->projTrans = glm::perspective(
    45.0f, // fov y
    800.0f / 600.0f, // aspect
    0.2f,  // near
    10.0f  //far
  );
  glUniformMatrix4fv(this->viewTransUniform,  1, GL_FALSE, glm::value_ptr(viewTrans));
  glUniformMatrix4fv(this->projTransUniform,  1, GL_FALSE, glm::value_ptr(projTrans));

  struct timeval t;
  gettimeofday(&t, NULL);
  this->startTime = t.tv_sec * 1000 + t.tv_usec / 1000;
 }

CubeScene::~CubeScene () {
  
}

void
CubeScene::Render () {
  struct timeval t;
  gettimeofday(&t, NULL);
  long int currentTime = t.tv_sec * 1000 + t.tv_usec / 1000;
  float time = (float) (currentTime - this->startTime) / 1000.0f;
      
  ///////////////////////////////////////////////////////////////////////////////////////
  // Draw the 3d scene
  // Let's render our set of vertices and along with it's attributes
  glBindVertexArray(this->vao);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glUseProgram(this->shaderProgram);

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

}
