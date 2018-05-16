#version 330 core

in vec2 tc;
uniform sampler2D tex;
out vec4 colour;
uniform vec2 texDimensions;
uniform vec4 textColour;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

// thx http://metalbyexample.com/rendering-text-in-metal-with-signed-distance-fields/
void main() {
  vec2 unit = 1.0 / textureSize(tex, 0);
  vec3 sample = texture(tex, tc).rgb;
  float dist = median(sample.r, sample.g, sample.b);
  float edgeWidth = 0.7 * length(vec2(dFdx(dist), dFdy(dist)));
  float opacity = smoothstep(0.5 - edgeWidth, 0.5 + edgeWidth, dist);
  vec4 background = vec4(textColour.rgb, 0.0);
  colour = mix(background, textColour, opacity);
  //colour = sigDist >= 0 ? textColour : background;
}
