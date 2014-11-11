#version 150

in vec3 inVertColor;
in vec2 inVertPosition;
in vec2 inVertTexCoord;

out vec3 outVertColor;
out vec2 outVertTexCoord;

void main () {
  gl_Position = vec4(inVertPosition, 0.0, 1.0);
  outVertColor = inVertColor;
  outVertTexCoord = inVertTexCoord;
}
