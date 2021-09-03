#version 460 core

layout(location = 1)
uniform sampler2DArray fragTexture;

in vec3 fragTexCoord;
in vec3 fragColor;

out vec4 outColor;

void main()
{
    outColor = vec4(fragColor, 1.0) * texture(fragTexture, fragTexCoord);
}