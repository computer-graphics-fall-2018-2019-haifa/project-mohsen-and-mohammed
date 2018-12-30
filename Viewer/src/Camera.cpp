#define _USE_MATH_DEFINES

#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Utils.h"
#define RADIAN(theta)  (2 * theta / 360.0f*M_PI)

Camera::Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) :
	_zoom(1.0),_height(0.0f),_aspectRatiop(0.0f), _aspectRatioo(0.0f), _fovy(0.0f),_nearo(0.0f),_faro(0.0f),
	_nearp(0.0f),_farp(0.0f),rotateX(0.0f),rotateY(0.0f),rotateZ(0.0f),eye(glm::vec4(0))
	,at(glm::vec4(0)),y(glm::vec4(0))
{
	this->eye = eye; this->at = at; this->y = y;
	SetCameraLookAt(Utils::SwitchFromHom(eye), Utils::SwitchFromHom(at), Utils::SwitchFromHom(up));
}
Camera::~Camera()
{
}
void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	this->eye = Utils::HomCoordinats(eye); this->at = Utils::HomCoordinats(at); this->y = Utils::HomCoordinats(up);
	glm::vec3 z = Utils::NormalizeVector(eye-at);
	glm::vec3 x = Utils::NormalizeVector(Utils::crossProduct(up, z));
	glm::vec3 y = Utils::NormalizeVector(Utils::crossProduct(z, x));
	this->InverseViewTransformation = Utils::Translate(glm::vec3(-1, -1, -1)*eye)*glm::mat4(x.x,x.y,x.z,0, y.x,y.y,y.z,0, z.x,z.y,z.z,0, 0,0,0,1);
	this->viewTransformation = glm::inverse(this->InverseViewTransformation);
}
void Camera::SetOrthographicProjection(
	const float height,
	const float aspectRatio,
	const float _near,
	const float _far)
{
	this->_height = height;
	this->_aspectRatioo = aspectRatio;
	this->_nearo = _near;
	this->_faro = _far;
	this->pro = ORTH;
	this->projectionTransformation = calculateOrthProjectionMatrix(height, aspectRatio, _near, _far);

}
void Camera::SetPerspectiveProjection(
	const float fovy,
	const float aspectRatio,
	const float near,
	const float far)
{
	this->_fovy = fovy;
	this->_aspectRatiop = aspectRatio;
	this->_nearp = near;
	this->_farp = far;
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
	return Utils::Scale(glm::vec3((2.0f) / (aspectRatio*height), (2.0f) / (height), (2.0f) / (fabs(far - near))));
}
glm::mat4 Camera::calculatePersProjetionMatrix(const float fovy, const float aspect, const float near, const float far)const {
	const float fovyR = RADIAN(fovy);//fovy in radians
	const float top = near * tanf(fovyR);
	const float bottom = -top;
	const float right=top*aspect;
	const float left=-right;
	return glm::mat4(near/right,0,0,0,
					 0,near/top,0,0,
					 0,0,-(near+far)/(far-near),-2*(far*near)/(far-near),
					 0,0,-1,0);
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
float Camera::getXRotate()const {
	return rotateX;
}
void Camera::setXRotate(const float t) {
	rotateX = t;
}
float Camera::getYRotate()const {
	return rotateY;
}
void Camera::setYRotate(const float t) {
	rotateY=t;
}
float Camera::getZRotate()const {
	return rotateZ;
}
void Camera::setZRotate(const float t) {
	rotateZ = t;
}
glm::vec3 Camera::getEye()const {
	return Utils::SwitchFromHom(eye);
}
void Camera::setEye(const glm::vec3& vec) {
	eye = Utils::HomCoordinats(vec);
}
glm::vec3 Camera::getAt()const {
	return Utils::SwitchFromHom(at);
}
void Camera::setAt(const glm::vec3& vec) {
	at = Utils::HomCoordinats(vec);
}
glm::vec3 Camera::getY()const {
	return Utils::SwitchFromHom(y);
}
void Camera::setY(const glm::vec3& vec) {
	y = Utils::HomCoordinats(vec);
}
float Camera::getFovy()const {
	return _fovy;
}
float Camera::getNearO()const {
	return _nearo;
}
float Camera::getFarO()const {
	return _faro;
}
float Camera::getNearP()const {
	return _nearp;
}
float Camera::getFarP()const {
	return _farp;
}
float Camera::getAspectO()const {
	return _aspectRatioo;
}
float Camera::getAspectP()const {
	return _aspectRatiop;
}
float Camera::getHeight()const {
	return _height;
}
float Camera::getZoom()const {
	return _zoom;
}
const glm::mat4& Camera::GetViewTransform()const {
	return viewTransformation;
}