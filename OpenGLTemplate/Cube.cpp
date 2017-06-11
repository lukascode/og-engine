#include "Cube.h"

static GLfloat vertices[] = {
	// Positions          
	-0.5f, -0.5f, -0.5f, 
	0.5f, -0.5f, -0.5f,  
	0.5f,  0.5f, -0.5f, 
	0.5f,  0.5f, -0.5f,  
	-0.5f,  0.5f, -0.5f, 
	-0.5f, -0.5f, -0.5f, 

	-0.5f, -0.5f,  0.5f, 
	0.5f, -0.5f,  0.5f, 
	0.5f,  0.5f,  0.5f, 
	0.5f,  0.5f,  0.5f, 
	-0.5f,  0.5f,  0.5f,  
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f, 
	-0.5f,  0.5f, -0.5f, 
	-0.5f, -0.5f, -0.5f, 
	-0.5f, -0.5f, -0.5f, 
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f, 

	0.5f,  0.5f,  0.5f, 
	0.5f,  0.5f, -0.5f,  
	0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f, 
	0.5f,  0.5f,  0.5f, 

	-0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f, -0.5f,  
	0.5f, -0.5f,  0.5f,  
	0.5f, -0.5f,  0.5f,  
	-0.5f, -0.5f,  0.5f, 
	-0.5f, -0.5f, -0.5f, 

	-0.5f,  0.5f, -0.5f, 
	0.5f,  0.5f, -0.5f, 
	0.5f,  0.5f,  0.5f,  
	0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f,  0.5f, 
	-0.5f,  0.5f, -0.5f
};

Cube::Cube(glm::mat4* projection, glm::vec3 position, glm::vec3 color, GLuint vbo)
{
	this->position = position;   
	this->color = color;

	scale_factor = glm::vec3(1.0f, 1.0f, 1.0f);
	rotation_angle_x = 0.0f;
	rotation_angle_y = 0.0f;
	rotation_angle_z = 0.0f;

	this->projection = projection;

	//vbo
	if (vbo == 0) {
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		_vbo = vbo;
	}
	else {
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		_vbo = vbo;
	}

	shader = ResourceManager::getInstance()->getShader("res/bbox.v.glsl", "res/bbox.f.glsl");

	//vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
}

const glm::mat4& Cube::getModel()
{
	glm::mat4 model = glm::mat4();
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation_angle_x), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(rotation_angle_y), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(rotation_angle_z), glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, scale_factor);
	return model;
}

void Cube::draw(Camera* camera)
{
	if (!camera) return;
	if (shader) {
		shader->use();
		glm::mat4 view_m = camera->View();
		glm::mat4 model_m = getModel();
		glUniformMatrix4fv(shader->getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model_m));
		glUniformMatrix4fv(shader->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view_m));
		glUniformMatrix4fv(shader->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(*projection));

		glBindVertexArray(vao);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_LINE_LOOP, 0, 36);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(0);
	}
}
