#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sys/time.h>

#define GLEW_STATIC
#include <GL/glew.h>

class FBO {
public:
  FBO();
  ~FBO();
  void Bind();
  void Render();

private:
  GLuint vao;
  GLuint fbo;
  GLuint vbo;

  GLuint shaderProgram;
};
