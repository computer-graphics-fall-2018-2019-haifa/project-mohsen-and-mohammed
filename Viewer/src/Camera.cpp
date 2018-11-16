#define _USE_MATH_DEFINES

#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Utils.h"

Camera::Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) :
	_zoom(1.0)
{
	SetCameraLookAt(eye, at, up);
}

Camera::~Camera()
{
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	glm::vec4 z = Utils::NormalizeVector(Utils::HomCoordinats(at) - Utils::HomCoordinats(eye));
	glm::vec4 x = Utils::NormalizeVector(Utils::HomCoordinats(Utils::crossProduct(up,z)));
	glm::vec4 y = Utils::NormalizeVector(Utils::HomCoordinats(Utils::crossProduct(z, x)));
	glm::vec4 t(0.0f, 0.0f, 0.0f, 1.0f);
	this->InverseViewTransformation=glm::mat4(x, y, z, t)*Utils::Translate(-eye);
	this->viewTransformation = glm::inverse(this->InverseViewTransformation);
}

void Camera::SetOrthographicProjection(
	const float height,
	const float aspectRatio,
	const float near,
	const float far)
{
	this->_height = height;
	this->_aspectRatio = aspectRatio;
	this->_near = near;
	this->_far = far;
	this->pro = ORTH;
	this->projectionTransformation = calculateOrthProjectionMatrix(height, aspectRatio, near, far);

}

void Camera::SetPerspectiveProjection(
	const float fovy,
	const float aspectRatio,
	const float near,
	const float far)
{
	this->_fovy = fovy;
	this->_aspectRatio = aspectRatio;
	this->_near = near;
	this->_far = far;
	this->pro = PERS;
	this->projectionTransformation = calculatePersProjetionMatrix(fovy,aspectRatio,near,far);
}

void Camera::SetZoom(const float zoom)
{
	this->_zoom = zoom;
}

bool Camera::CanBeSeen(const std::vector<glm::vec3>& traingles) const {
	int flag = 1;
	for (int i = 0; i < traingles.size(); i++) {
		//first we calcuate every vertex coordinates in the camera frame
		glm::vec4 cameraCoor = this->InverseViewTransformation*Utils::HomCoordinats(traingles.at(i));
		//second we check if the vertix exist in the view volume or must be clipped
		if (!AUX_CanBeSeen(cameraCoor)) {
			flag *= 0;
		}
	}
	return flag;
}

glm::mat4 Camera::calculateOrthProjectionMatrix(const float height, const float aspectRatio, const float near, const float far) const {
	glm::vec4 row1 = glm::vec4((2.0f)/(aspectRatio*height),0,0,0);
	glm::vec4 row2 = glm::vec4((2.0f) / (height), 0, 0, 0);
	glm::vec4 row3 = glm::vec4((-2.0f) / (far-near), 0, 0, 0);
	glm::vec4 row4 = glm::vec4(0, 0, 0, 1);
	return glm::mat4(row1, row2, row3, row4);
}

glm::mat4 Camera::calculatePersProjetionMatrix(const float fovy, const float aspect, const float near, const float far)const {
	glm::vec4 row1 = glm::vec4(0,0,0,0);
	glm::vec4 row2 = glm::vec4(0, 0, 0, 0);
	glm::vec4 row3 = glm::vec4(0, 0, 0, 0);
	glm::vec4 row4 = glm::vec4(0, 0, 0, 0);
	return glm::mat4(row1, row2, row3, row4);
}

bool Camera::AUX_CanBeSeen(const glm::vec4& point)const {
	return true;
}