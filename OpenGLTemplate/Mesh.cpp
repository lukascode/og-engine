#include "Mesh.h"

Vertex::Vertex()
{
	position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	normal = glm::vec3(0.0f, 0.0f, 0.0f);
	texCoord = glm::vec2(0.0f, 0.0f);
}

Vertex::Vertex(glm::vec4 position, glm::vec3 normal, glm::vec2 texCoord)
{
	this->position = position;
	this->normal = normal;
	this->texCoord = texCoord;
}

Texture::Texture()
{
	id = 0;
}

Texture::Texture(GLuint id, std::string type)
{
	this->id = id;
	this->type = type;
}

Material::Material()
{
	ambient = glm::vec3(0.1f);
	diffuse = glm::vec3(0.5f);
	specular = glm::vec3(1.0f);
	shininess = 32.0f;
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material material, std::string name)
{
	this->vertices = vertices;
	this->indices = indices;
	this->name = name;
	this->material = material;
	prepareMesh();
}

void Mesh::prepareMesh()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	//positions
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	//normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	//textures
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Mesh::draw(ShaderProgram* shader)
{
	
	int ambient = 1, diffuse = 1, specular = 1, nr;
	for (int i = 0; i < material.textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string type = material.textures[i].type;
		if (type == "texture_ambient") { nr = ambient; ++ambient; }
		if (type == "texture_diffuse") { nr = diffuse; ++diffuse; }
		if (type == "texture_specular") { nr = specular; ++specular; }
		shader->set1f("material." + type + std::to_string(nr), i);
		glBindTexture(GL_TEXTURE_2D, material.textures[i].id);
	} 
	glActiveTexture(GL_TEXTURE0);
	

	shader->set3f("material.ambient", material.ambient);
	shader->set3f("material.diffuse", material.diffuse);
	shader->set3f("material.specular", material.specular);
	shader->set1f("material.shininess", material.shininess);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

std::string Mesh::getName()
{
	return name;
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
}

