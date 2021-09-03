#version 460 core

layout(location = 0)
uniform mat4 mp;

layout(location = 0)
in vec3 inPosition;

layout(location = 1)
in vec2 inTexCoord;

out vec2 fragTexCoord;

void main()
{
   gl_Position = mp * vec4(inPosition.xyz, 1.0);
   fragTexCoord = inTexCoord;
}