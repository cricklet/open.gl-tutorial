#include <string>
#include <fstream>
#include <streambuf>

#include <iostream>
#include <stdexcept>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <SOIL/soil.h>

bool checkErrors(const char *filename, int line) {
  bool result = false;
  
  GLenum error;
  while ((error = glGetError()) != GL_NO_ERROR) {
    std::string str;
 
    switch(error) {
    case GL_INVALID_OPERATION:
      str="INVALID_OPERATION"; break;
    case GL_INVALID_ENUM:
      str="INVALID_ENUM"; break;
    case GL_INVALID_VALUE:
      str="INVALID_VALUE"; break;
    case GL_OUT_OF_MEMORY:
      str="OUT_OF_MEMORY"; break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      str="INVALID_FRAMEBUFFER_OPERATION";  break;
    }
 
    // std::cerr << "GL_" << str.c_str() << "\n";
    std::cerr << "GL_" << str.c_str() << " - "<< filename << ":" << line << "\n";
    result = true;
  }

  return result;
}
#define checkErrors() checkErrors(__FILE__, __LINE__)

GLuint loadTexture(const char *filename, GLuint &tex, int index) {
  glActiveTexture(GL_TEXTURE0 + index);
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

char *getFileContents(const char *filename) {
  FILE *file;
  long fileSize;
  char *buffer; // this can get leaked if an exception is thrown before it's returned

  file = fopen ( filename , "rb" );
  if (!file) {
    throw std::runtime_error(std::string("Failed to open file: ") + filename);
  }
  
  // how big is the file?
  fseek(file , 0L , SEEK_END);
  fileSize = ftell(file);
  rewind(file);

  // make a buffer big enough to hold the file
  buffer = (char *) calloc(fileSize + 1, 1);
  if(!buffer) {
    throw std::runtime_error(std::string("Failed to create buffer for: ") + filename);
  }

  // copy the file into the buffer
  int result = fread(buffer, fileSize, 1, file);
  if (result != 1) {
    throw std::runtime_error(std::string("Failed to copy file: ") + filename);
  }
  
  fclose(file);
  return buffer;
}

GLuint compileShader (const char *filename, GLenum shaderType) {
  char *fileContents;
  try {
    fileContents = getFileContents(filename);
  } catch (std::exception &e) {
    std::cerr << "Exception caught: " << e.what() << "\n";
  }

  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &fileContents, NULL);
  glCompileShader(shader);
  
  // Print shader compile errors.
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    std::cerr << "Failed to compile shader\n";
    char buffer[512];
    glGetShaderInfoLog(shader, 512, NULL, buffer);
    std::cerr << buffer << "\n";

    throw std::runtime_error(std::string("Failed to compile shader: ") + filename);
  }

  free(fileContents);
  return shader;
}

int main (int argv, char *argc[]) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_Window *window = SDL_CreateWindow("OpenGL", 100, 100, 640, 480, SDL_WINDOW_OPENGL);
  SDL_GLContext context = SDL_GL_CreateContext(window);

  glewExperimental = GL_TRUE; // necessary for modern opengl calls
  glewInit();
  checkErrors();

  GLfloat vertices[] = {
    0.5f,  0.5f,  1,0,0, 0,0, // position, rgb, tex-coord
    0.5f, -0.5f,  0,1,0, 0,1,
    -0.5f, -0.5f, 0,0,1, 1,1,
    -0.5f, 0.5f,  1,0,1, 1,0
  };

  GLuint vertexStride = sizeof(GLfloat) * 7;
  void *positionOffset = 0;
  void *colorOffset = (void *) (2 * sizeof(GLfloat));
  void *texOffset   = (void *) (5 * sizeof(GLfloat));

  GLuint elements[] = {
    0, 1, 2, 3, 0, 2
  };

  GLuint numElements = 6;

  // We don't want to have to call glVertexAttribPointer to reset the inputs every time we enable
  // a shader (glUseProgram). Instead, we can store all the state needed to use a shader inside
  // a vertex array object.
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  checkErrors();

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

  // We want to be able to render the vertices in any order (with repetition). To do this, we
  // create an element array buffer object.
  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
  checkErrors();

  // We can store 2d textures in the same kind of buffer.
  GLuint textures[2];
  glGenTextures(2, textures);
  
  int texKittenIndex = 0;
  GLuint texKitten = loadTexture("kitten.png", textures[texKittenIndex], texKittenIndex);

  int texPuppyIndex = 1;
  GLuint texPuppy = loadTexture("puppy.png", textures[texPuppyIndex], texPuppyIndex);

  // Load the shaders from the filesystem.
  GLuint vertShader = compileShader("screen.vert", GL_VERTEX_SHADER);
  GLuint fragShader = compileShader("screen.frag", GL_FRAGMENT_SHADER);
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
  glUseProgram(shaderProgram);
  checkErrors();
  
  // Shader needs to know how to get the input attributes.
  GLint posAttrib = glGetAttribLocation(shaderProgram, "inVertPosition");
  glVertexAttribPointer(posAttrib,
			2,  // size of vector i.e. vec2
			GL_FLOAT,
			GL_FALSE, // should be normalized
			vertexStride,  // stride: # bytes between each attribute in the array
			0); // offset: # bytes from the start of the array
  glEnableVertexAttribArray(posAttrib);

  GLint colorAttrib = glGetAttribLocation(shaderProgram, "inVertColor");
  glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE,
			vertexStride,
			colorOffset);
  glEnableVertexAttribArray(colorAttrib);

  GLint texCoordAttrib = glGetAttribLocation(shaderProgram, "inVertTexCoord");
  glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE,
			vertexStride,
			texOffset);
  glEnableVertexAttribArray(texCoordAttrib);

  GLint texKittenAttrib = glGetAttribLocation(shaderProgram, "texKitten");
  glUniform1i(texKittenAttrib, texKittenIndex);

  GLint texPuppyAttrib = glGetAttribLocation(shaderProgram, "texPuppy");
  glUniform1i(texPuppyAttrib, texPuppyIndex);

  // GLint colorUniform = glGetUniformLocation(shaderProgram, "triangleColor");
  // glUniform3f(colorUniform, 1.0f, 0.0f, 0.0f);

  checkErrors();

  SDL_Event windowEvent;
  while (true) {
    if (SDL_PollEvent(&windowEvent)) {
      if (windowEvent.type == SDL_QUIT) break;
      if (windowEvent.type == SDL_KEYUP) {
	if (windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
      }
    }

    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Vary the color attribute
    // float time = (float) clock() / (float) CLOCKS_PER_SEC;
    // glUniform3f(colorUniform, (sin(100 * time) + 1.0f) / 2.0f, 0.0f, 0.0f);

    // glDrawArrays(GL_TRIANGLES, 0 /*skip*/, 3 /*num*/);
    glDrawElements(GL_TRIANGLES, numElements /*num*/, GL_UNSIGNED_INT, 0 /*offset*/);

    SDL_GL_SwapWindow(window);
    checkErrors();
  }

  glDeleteTextures(1, &texKitten);

  glDeleteProgram(shaderProgram);
  glDeleteShader(fragShader);
  glDeleteShader(vertShader);

  glDeleteBuffers(1, &ebo);
  glDeleteBuffers(1, &vbo);

  glDeleteVertexArrays(1, &vao);

  SDL_GL_DeleteContext(context);
  SDL_Quit();  
  return 0;
}
