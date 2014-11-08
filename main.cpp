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
  // try {
  fileContents = getFileContents(filename);
  // } catch (std::exception &e) {
  //   std::cerr << "Exception caught: " << e.what() << "\n";
  // }

  GLuint shader = glCreateShader(shaderType);
  glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(shader, 1, &fileContents, NULL);
  glCompileShader(shader);

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

  GLuint vertexBuffer; // test that glew loaded
  glGenBuffers(1, &vertexBuffer);
  std::cout << vertexBuffer << "\n";

  float vertices[] = {
    0.0f,  0.5f, // Vertex 1 (X, Y)
    0.5f, -0.5f, // Vertex 2 (X, Y)
    -0.5f, -0.5f  // Vertex 3 (X, Y)
  };

  GLuint vbo; // vertex buffer object
  glGenBuffers(1, &vbo); // generate one buffer object name
  glBindBuffer(GL_ARRAY_BUFFER, // target to bind to
	       vbo);

  glBufferData(GL_ARRAY_BUFFER, // target buffer object
	       sizeof(vertices), vertices,
	       GL_STATIC_DRAW); // vs GL_DYNAMIC_DRAW vs GL_STREAM_DRAW

  // Load the shaders from the filesystem.
  GLuint vertShader = compileShader("screen.vert", GL_VERTEX_SHADER);
  GLuint fragShader = compileShader("screen.frag", GL_VERTEX_SHADER);
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertShader);
  glAttachShader(shaderProgram, fragShader);

  // Because fragment shaders can often write to multiple buffers, we need to specify which buffer
  // to bind the user-defined varying 'out' variable to.
  glBindFragDataLocation(shaderProgram,
			 0, // color number (buffer) to bind to
			 "outColor");

  glLinkProgram(shaderProgram);
  
  // Shader needs to know how to get the input attributes.
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(posAttrib,
			2,  // size of vector i.e. vec2
			GL_FLOAT,
			GL_FALSE, // should be normalized
			0,  // stride: # bytes between each attribute in the array
			0); // offset: # bytes from the start of the array

  glEnableVertexAttribArray(posAttrib);

  glUseProgram(shaderProgram);

  SDL_Event windowEvent;
  while (true) {
    if (SDL_PollEvent(&windowEvent)) {
      if (windowEvent.type == SDL_QUIT) break;
      if (windowEvent.type == SDL_KEYUP) {
	if (windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
      }
    }

    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(context);
  SDL_Quit();  
  return 0;
}
