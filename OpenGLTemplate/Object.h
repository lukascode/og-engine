#pragma once
#include "ShaderProgram.h"
#include "Camera.h"
#include "Model.h"
#include "Cube.h"

class Object
{
public:
	glm::vec3 position;
	GLfloat rotation_angle_x; //radians
	GLfloat rotation_angle_y;
	GLfloat rotation_angle_z;
	glm::vec3 scale_factor;

	Object(std::string model, glm::mat4* projection, ShaderProgram* shader, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
	Object(Model* model, glm::mat4* projection, ShaderProgram* shader, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
	void draw(Camera* camera, glm::mat4 parent_model);
	

	glm::mat4 getModel();
	glm::mat4 cache_parent_model;
	BBox getBBox();
private:
	Model* model;
	ShaderProgram* shader;
	glm::mat4* projection;
	Cube* cube;

	void drawBBox(Camera* camera);
};