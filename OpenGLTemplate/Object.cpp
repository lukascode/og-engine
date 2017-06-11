#include "Object.h"
#include "ResourceManager.h"

Object::Object(std::string model, glm::mat4* projection, ShaderProgram* shader, glm::vec3 position)
{
	this->position = position;
	this->projection = projection;
	scale_factor = glm::vec3(1.0f, 1.0f, 1.0f);
	rotation_angle_x = 0.0f;
	rotation_angle_y = 0.0f;
	rotation_angle_z = 0.0f;

	this->model = ResourceManager::getInstance()->getModel(model);
	this->shader = shader;
	this->cache_parent_model = glm::mat4(1.0f);

	cube = new Cube(projection);
}

Object::Object(Model* model, glm::mat4* projection, ShaderProgram* shader, glm::vec3 position)
{
	this->model = model;
	this->position = position;
	this->projection = projection;
	scale_factor = glm::vec3(1.0f, 1.0f, 1.0f);
	rotation_angle_x = 0.0f;
	rotation_angle_y = 0.0f;
	rotation_angle_z = 0.0f;
	this->shader = shader;
	this->cache_parent_model = glm::mat4(1.0f);
	cube = new Cube(projection);
}

glm::mat4 Object::getModel()
{
	glm::mat4 model = glm::mat4();
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation_angle_x), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(rotation_angle_y), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(rotation_angle_z), glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, scale_factor);
	return model;
}

void Object::draw(Camera* camera, glm::mat4 parent_model)
{
	if (!camera || !model) return;
	if (shader) {
		shader->use();
		glm::mat4 view_m = camera->View();
		glm::mat4 model_m = getModel();
		model_m = parent_model * model_m;
		glUniformMatrix4fv(shader->getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model_m));
		glUniformMatrix4fv(shader->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view_m));
		glUniformMatrix4fv(shader->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(*projection));
		model->draw(shader);
		cache_parent_model = parent_model;
	}
	drawBBox(camera);
}

BBox Object::getBBox()
{
	BBox bbox = model->getBBox();
	glm::mat4 model_m = cache_parent_model * getModel();
	bbox.max =  model_m * bbox.max;
	bbox.min = model_m * bbox.min;
	bbox.center = model_m * bbox.center;
	return bbox;
} 


void Object::drawBBox(Camera* camera)
{
	BBox bbox = getBBox();
	cube->position = bbox.center;
	cube->scale_factor.x = bbox.max.x - bbox.min.x;
	cube->scale_factor.y = bbox.max.y - bbox.min.y;
	cube->scale_factor.z = bbox.max.z - bbox.min.z;
	cube->draw(camera);
}