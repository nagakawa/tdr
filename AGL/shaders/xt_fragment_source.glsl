#version 330 core

in vec2 tc;
uniform sampler2D tex;
out vec4 colour;
uniform vec2 texDimensions;
uniform vec4 textColour;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
  vec2 msdfUnit = 0.5 / texDimensions;
  vec3 sample = texture(tex, tc).rgb;
  float sigDist = median(sample.r, sample.g, sample.b) - 0.5;
  sigDist *= dot(msdfUnit, 0.5 / fwidth(tc));
  float opacity = clamp(sigDist + 0.5, 0.0, 1.0);
  vec4 background = vec4(textColour.rgb, 0.0);
  colour = mix(background, textColour, opacity);
  //colour = sigDist >= 0 ? textColour : background;
}
