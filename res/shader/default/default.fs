#version 460 core

layout(location = 1)
uniform sampler2D fragTexture;

in vec2 fragTexCoord;

out vec4 outColor;

void main()
{
    outColor = texture(fragTexture, fragTexCoord);
}