#version 460 core

layout(location = 1)
uniform sampler2D fragTexture;
layout(location = 2)
uniform vec4 bgColor;
layout(location = 3)
uniform vec4 fgColor;
layout(location = 4)
uniform float unitRange;

in vec2 fragTexCoord;

out vec4 outColor;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

float screenPxRange() {
    vec2 screenTextureSize = vec2(1.0)/fwidth(texCoord);
    return max(0.5*dot(unitRange, screenTextureSize), 1.0);
}
void main() {
    vec3 msd = texture(fragTexture, fragTexCoord).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    outColor = mix(bgColor, fgColor, opacity);
}