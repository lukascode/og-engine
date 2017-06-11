#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

enum CMOVE {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	void MotionHandler(GLfloat xoffset, GLfloat yoffset); //change direction depends on motion
	void move(CMOVE direction, GLfloat dT);
	void setPosition(glm::vec3 position);
	void setRotation(GLfloat yaw, GLfloat pitch);
	GLfloat getZoom();
	void zoomIn();
	void zoomOut();
	void zoomReset();
	glm::mat4 View();

	glm::vec3 position;
	GLfloat viewScopeRadius;
private:
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;

	//e angles
	GLfloat yaw;
	GLfloat pitch;

	GLfloat zoom;
	GLfloat movementSpeed;

	void Zoom(GLfloat zoomUnit);
	void calculateVectors();

};

