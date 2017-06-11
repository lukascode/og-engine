#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgram.h"
#include "ResourceManager.h"
#include "Camera.h"

class Cube
{
public:
	Cube(glm::mat4* projection, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), GLuint vbo = 0);

	glm::vec3 position;
	glm::vec3 scale_factor;
	GLfloat rotation_angle_x; //degrees
	GLfloat rotation_angle_y;
	GLfloat rotation_angle_z;

	glm::vec3 color;
	GLuint _vbo;

	const glm::mat4& getModel();
	void draw(Camera* camera);

private:
	GLuint vao;
	ShaderProgram* shader;
	glm::mat4* projection;

};