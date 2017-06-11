#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPosition;
out vec3 Normal;
out vec2 TexCoord;


void main() 
{
	gl_Position = projection * view * model * position;
	TexCoord = texCoord;
	Normal = mat3(transpose(inverse(model))) * normal;
	FragPosition = vec3(model * position);
}