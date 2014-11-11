#version 150

in vec3 outVertColor;
in vec2 outVertTexCoord;

out vec4 outFragColor;

uniform sampler2D tex;
// uniform vec3 triangleColor;

void main() {
  outFragColor = vec4(outVertColor, 1.0) * texture(tex, outVertTexCoord);
  // vec4(triangleColor, 1.0);
  //vec4(1.0, 1.0, 1.0, 1.0);
}
