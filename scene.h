#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sys/time.h>

#define GLEW_STATIC
#include <GL/glew.h>

class CubeScene {
public:
  CubeScene();
  ~CubeScene();
  void Render(float t);

private:
  GLuint vao;
  GLuint vbo;

  GLuint texKitten;
  GLuint texPuppy;

  GLuint shaderProgram;

  GLint overrideColor;
  GLint timeUniform;
  GLint modelTransUniform;
  GLint viewTransUniform;
  GLint projTransUniform;

  glm::mat4 viewTrans;
  glm::mat4 projTrans;

  long int startTime;
};
