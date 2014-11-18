#define GLEW_STATIC
#include <GL/glew.h>

class CubeScene {
public:
  CubeScene();
  ~CubeScene();
  //void Render();

private:
  GLuint vao;
  GLuint vbo;
  GLuint texKitten;
  GLuint texPuppy;
};
