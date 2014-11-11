#version 150

in vec3 inVertColor;
in vec2 inVertPosition;

out vec3 outVertColor;

void main () {
  gl_Position = vec4(inVertPosition, 0.0, 1.0);
  outVertColor = inVertColor;
}
