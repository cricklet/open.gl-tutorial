#version 150

in vec2 outVertBufferCoord;

out vec4 outFragColor;

uniform sampler2D buffer;

const float blurSizeH = 1.0 / 300.0;
const float blurSizeV = 1.0 / 200.0;

void main() {
  vec4 result = vec4(0,0,0,1);
  for (int x = -4; x <= 4; x ++) {
    for (int y = -4; y <= 4; y ++) {
      vec2 loc = outVertBufferCoord + vec2(x * blurSizeW, y * blurSizeH);
      result += texture(buffer, loc) / 81.0;
    }
  }

  // outFragColor = vec4(1,1,1,1) - texture(buffer, outVertBufferCoord);
  outFragColor = result;
}
