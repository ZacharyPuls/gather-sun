#version 460 core

layout(location = 0)
uniform mat4 mp;

layout(location = 0)
in vec3 inPosition;

layout(location = 1)
in vec3 inTexCoord;

layout(location = 2)
in vec3 inColor;

out vec3 fragTexCoord;
out vec3 fragColor;

void main()
{
   gl_Position = mp * vec4(inPosition.xyz, 1.0);
   fragTexCoord = inTexCoord;
   fragColor = inColor;
}