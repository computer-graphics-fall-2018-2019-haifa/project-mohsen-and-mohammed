#define _USE_MATH_DEFINES

#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Utils.h"

Camera::Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) :
	_zoom(1.0)
{
	SetCameraLookAt(Utils::SwitchFromHom(eye), Utils::SwitchFromHom(at), Utils::SwitchFromHom(up));
}

Camera::~Camera()
{
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	glm::vec3 z = Utils::NormalizeVector(eye-at);
	glm::vec3 x = Utils::NormalizeVector(Utils::crossProduct(up, z));
	glm::vec3 y = Utils::NormalizeVector(Utils::crossProduct(z, x));
	glm::vec4 t(0.0f, 0.0f, 0.0f, 1.0f);
	this->InverseViewTransformation = glm::mat4(x.x,x.y,x.z,0, y.x,y.y,y.z,0, z.x,z.y,z.z,0, 0,0,0,1)*Utils::Translate(glm::vec3(-1,-1,-1)*eye);
	glm::mat4 temp1 = this->InverseViewTransformation;
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
	this->projectionTransformation = calculatePersProjetionMatrix(fovy, aspectRatio, near, far);
}

void Camera::SetZoom(const float zoom)
{
	this->_zoom = zoom;
}

bool Camera::CanBeSeen(const std::vector<glm::vec4>& traingles) const {
	int flag = 1;
	for (int i = 0; i < traingles.size(); i++) {
		//first we calcuate every vertex coordinates in the camera frame
		//glm::vec4 cameraCoor = this->InverseViewTransformation*Utils::HomCoordinats(traingles.at(i));
		//second we check if the vertix exist in the view volume or must be clipped
		if (!AUX_CanBeSeen(traingles.at(i))) {
			flag *= 0;
		}
	}
	return flag;
}

glm::mat4 Camera::calculateOrthProjectionMatrix(const float height, const float aspectRatio, const float near, const float far) const {
	/*typedef glm::vec4 myVec;
	myVec row1 = myVec((2.0f) / (aspectRatio*height), 0, 0, 0);
	myVec row2 = myVec(0, (2.0f) / (height), 0, 0);
	myVec row3 = myVec(0, 0, (-2.0f) / (far - near), 0);
	myVec row4 = myVec(0, 0, 0, 1);*/
	return Utils::Scale(glm::vec3((2.0f) / (aspectRatio*height), (2.0f) / (height), (2.0f) / (fabs(far - near))));
}

glm::mat4 Camera::calculatePersProjetionMatrix(const float fovy, const float aspect, const float near, const float far)const {
	return glm::mat4(1);
}

bool Camera::AUX_CanBeSeen(const glm::vec4& point)const {
	glm::vec4 temp = point;
	return ((temp.x >= -1 && temp.x <= 1) && (temp.y >= -1 && temp.y <= 1) && (temp.z >= -1 && temp.z <= 1));
}
const glm::mat4& Camera::GetInverseViewTranform()const {
	return this->InverseViewTransformation;
}
const glm::mat4& Camera::GetProjectionTransform()const {
	return this->projectionTransformation;
}