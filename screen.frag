#version 150

in vec3 outVertColor;
in vec2 outVertTexCoord;

out vec4 outFragColor;

uniform sampler2D texKitten;
uniform sampler2D texPuppy;
// uniform vec3 triangleColor;

void main() {
  vec4 colorKitten = texture(texKitten, outVertTexCoord);
  vec4 colorPuppy  = texture(texPuppy,  outVertTexCoord);
  outFragColor = vec4(outVertColor, 1.0) * mix(colorKitten, colorPuppy, 0.5);
  // vec4(triangleColor, 1.0);
  //vec4(1.0, 1.0, 1.0, 1.0);
}
