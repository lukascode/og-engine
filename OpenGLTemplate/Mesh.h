#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>
#include "ShaderProgram.h"

struct Vertex
{
	glm::vec4 position;
	glm::vec3 normal;
	glm::vec2 texCoord;

	Vertex();
	Vertex(glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		glm::vec3 normals = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec2 texCoord = glm::vec2(0.0f, 0.0f));
};

struct Texture
{
	GLuint id;
	std::string type;

	Texture();
	Texture(GLuint id, std::string type);
};

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	std::vector<Texture> textures;

	Material();
};

class Mesh
{
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material material, std::string name = "unnamed");
	~Mesh();
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices; //~faces
	Material material;
	
	void draw(ShaderProgram* shader);
	std::string getName();
	
private:
	GLuint vao, vbo, ebo;
	std::string name;
	void prepareMesh();
};



