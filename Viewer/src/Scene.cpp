#include "Scene.h"
#include "MeshModel.h"

#include <iostream>
#include <string>

Scene::Scene() :
	activeCameraIndex(0),
	activeModelIndex(0), 
	ambientColor(glm::vec3(0))
{

}
void Scene::AddModel(const std::shared_ptr<MeshModel>& model)
{
	models.push_back(model);
}
const int Scene::GetModelCount() const
{
	return models.size();
}
void Scene::AddCamera(const Camera& camera)
{
	cameras.push_back(camera);
}
const int Scene::GetCameraCount() const
{
	return cameras.size();
}
void Scene::SetActiveCameraIndex(int index)
{
	// implementation suggestion...
	if (index >= 0 && index < cameras.size())
	{
		activeCameraIndex = index;
	}
}
const int Scene::GetActiveCameraIndex() const
{
	return activeCameraIndex;
}
void Scene::SetActiveModelIndex(int index)
{
	// implementation suggestion...
	if (index >= 0 && index < models.size())
	{
		activeModelIndex = index;
	}
}
const int Scene::GetActiveModelIndex() const
{
	return activeModelIndex;
}
std::shared_ptr<MeshModel> Scene::GetAciveModel()const {
	return models.at(this->GetActiveModelIndex());
}
const Camera& Scene::GetActiveCamera()const {
	return cameras.at(this->GetActiveCameraIndex());
}
void Scene::UpdateActiveCameraXRotate(const float x) {
	cameras.at(this->GetActiveCameraIndex()).setXRotate(x);
}
float Scene::GetActiveCameraXRotate()const {
	return cameras.at(activeCameraIndex).getXRotate();
}
float Scene::GetActiveCameraYRotate()const {
	return cameras.at(activeCameraIndex).getYRotate();
}
void Scene::UpdateActiveCameraYRotate(const float y) {
	cameras.at(activeCameraIndex).setYRotate(y);
}
float Scene::GetActiveCameraZRotate()const {
	return cameras.at(activeCameraIndex).getZRotate();
}
void Scene::UpdateActiveCameraZRotate(const float z) {
	cameras.at(activeCameraIndex).setZRotate(z);
}
void Scene::ActiveCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up) {
	cameras.at(activeCameraIndex).SetCameraLookAt(eye, at, up);
}

void Scene::ActiveCamerSetOrth(const float height,
	const float aspectRatio,
	const float _near,
	const float _far) {
	cameras.at(activeCameraIndex).SetOrthographicProjection(height, aspectRatio, _near, _far);
}
void Scene::ActiveCameraSerPers(const float fovy,
	const float aspect,
	const float _near,
	const float _far) {
	cameras.at(activeCameraIndex).SetPerspectiveProjection(fovy, aspect, _near, _far);
}
std::shared_ptr<MeshModel> Scene::GetModelIndex(const int index)const {
	if (index < 0 || index >= this->models.size()) { std::cout << "WTF!" << std::endl; exit(1); }
	return models.at(index);
}
const Camera& Scene::GetCameraIndex(const int index)const {
	if (index <0|| index >= cameras.size()) { std::cout << "WTF!" << std::endl; exit(1); }
	return cameras.at(index);
}


void Scene::UpdateActiveCameraTilt(const float x, AXIS a) {
	if (a == X)
		cameras.at(this->GetActiveCameraIndex()).setTiltX(x);
	else if (a == Y)
		cameras.at(this->GetActiveCameraIndex()).setTiltY(x);
	else if (a == Z)
		cameras.at(this->GetActiveCameraIndex()).setTiltZ(x);
}

void Scene::setAmbient(const glm::vec3& newColor) {
	ambientColor = newColor;
}
glm::vec3 Scene::getAmbient()const {
	return ambientColor;
}