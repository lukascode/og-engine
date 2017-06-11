#include "Camera.h"

//Default configuration
static const GLfloat PITCH = 0.0f;
static const GLfloat YAW = -90.0f;
static const GLfloat ZOOM = 45.0f;
static const GLfloat MOVEMENT_SPEED = 25.0f;
static const GLfloat MOTION_SENSITIVITY = 0.25f;
static const GLfloat ZOOM_UNIT = 1.0f;
static const GLfloat VIEW_SCOPE_RADIUS = 60.0f;

Camera::Camera(glm::vec3 position, glm::vec3 up) : zoom(ZOOM), front(glm::vec3(0.0, 0.0, -1.0))
{
	this->position = position;
	this->up = up;
	this->yaw = YAW;
	this->pitch = PITCH;
	this->movementSpeed = MOVEMENT_SPEED;
	this->worldUp = up;
	this->viewScopeRadius = VIEW_SCOPE_RADIUS;
	calculateVectors();
}

void Camera::setPosition(glm::vec3 position)
{
	this->position = position;
}

void Camera::setRotation(GLfloat yaw, GLfloat pitch)
{
	this->yaw = yaw;
	this->pitch = pitch;
	calculateVectors();
}

void Camera::MotionHandler(GLfloat xoffset, GLfloat yoffset)
{
	xoffset *= MOTION_SENSITIVITY;
	yoffset *= MOTION_SENSITIVITY;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	calculateVectors();
}

void Camera::move(CMOVE direction, GLfloat dT)
{
	switch (direction) {
	case FORWARD:
		position += front * (movementSpeed*dT);
		break;
	case BACKWARD:
		position -= front * (movementSpeed*dT);
		break;
	case LEFT:
		position -= right * (movementSpeed*dT);
		break;
	case RIGHT:
		position += right * (movementSpeed*dT);
		break;
	}
	//this->front.y = 0.0f; //camera 'll not fly
}

GLfloat Camera::getZoom()
{
	return zoom;
}

void Camera::zoomIn()
{
	Zoom(ZOOM_UNIT);
}

void Camera::zoomOut()
{
	Zoom(-ZOOM_UNIT);
}

void Camera::Zoom(GLfloat zoomUnit)
{
	if (zoom >= 1.0f && zoom <= ZOOM) 
		zoom -= zoomUnit;
	if (zoom <= 1.0f)
		zoom = 1.0f;
	if (zoom >= ZOOM)
		zoom = ZOOM;
}

void Camera::zoomReset()
{
	zoom = ZOOM;
}

//view matrix
glm::mat4 Camera::View()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::calculateVectors()
{
	glm::vec3 front_vec;
	front_vec.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front_vec.y = sin(glm::radians(pitch));
	front_vec.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front_vec);
	//recalculate right and up vector
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
