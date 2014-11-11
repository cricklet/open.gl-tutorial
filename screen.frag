#version 150

in vec3 outVertColor;
out vec4 outFragColor;

// uniform vec3 triangleColor;

void main() {
  outFragColor = vec4(outVertColor, 1.0);
  // vec4(triangleColor, 1.0);
  //vec4(1.0, 1.0, 1.0, 1.0);
}
