#version 150

in vec3 outVertColor;
in vec2 outVertTexCoord;

out vec4 outFragColor;

uniform sampler2D texKitten;
uniform sampler2D texPuppy;
uniform float time;

void main() {
  vec2 texCoord = outVertTexCoord;
  texCoord += 0.02 * vec2(sin(texCoord.y * 20 + time * 5), 0);
  
  vec4 colorKitten = texture(texKitten, texCoord);
  vec4 colorPuppy  = texture(texPuppy,  texCoord);
  outFragColor = vec4(outVertColor, 1.0) * mix(colorKitten, colorPuppy, 0.5 + 0.5 * sin(time));
  // vec4(triangleColor, 1.0);
  //vec4(1.0, 1.0, 1.0, 1.0);
}
