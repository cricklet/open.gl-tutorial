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
    0.0f,  0.5f,
    0.5f, -0.5f,
    -0.5f, -0.5f
  };

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
			 "outColor");
  checkErrors();

  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);
  checkErrors();
  
  // Shader needs to know how to get the input attributes.
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(posAttrib,
			2,  // size of vector i.e. vec2
			GL_FLOAT,
			GL_FALSE, // should be normalized
			0,  // stride: # bytes between each attribute in the array
			0); // offset: # bytes from the start of the array
  glEnableVertexAttribArray(posAttrib);
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

    glDrawArrays(GL_TRIANGLES,
		 0,  // # vertices to skip at the beginning
		 3); // # verticies to process

    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(context);
  SDL_Quit();  
  return 0;
}
