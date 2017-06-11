#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragmentPosition;

void main() 
{
    Normal = mat3(transpose(inverse(model))) * normal; 
    gl_Position = projection * view * model * vec4(position, 1.0f);
    FragmentPosition = vec3(model * vec4(position, 1.0f));
}