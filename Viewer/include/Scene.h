#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Camera.h"

/*
 * Scene class.
 * This class holds all the scene information (models, cameras, lights, etc..)
 */
class Scene {
private:
	std::vector<std::shared_ptr<MeshModel>> models;
	std::vector<Camera> cameras;

	int activeCameraIndex;
	int activeModelIndex;

public:
	Scene();

	void AddModel(const std::shared_ptr<MeshModel>& model);
	const int GetModelCount() const;

	void AddCamera(const Camera& camera);
	const int GetCameraCount() const;

	void SetActiveCameraIndex(int index);
	const int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	const int GetActiveModelIndex() const;

	std::shared_ptr< MeshModel> GetAciveModel()const;

	const Camera& GetActiveCamera()const;
	void UpdateActiveCameraXRotate(const float x);

	float GetActiveCameraXRotate()const;
	void UpdateActiveCameraYRotate(const float x);
	float GetActiveCameraYRotate()const;
	void UpdateActiveCameraZRotate(const float x);
	float GetActiveCameraZRotate()const;

	void ActiveCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	void ActiveCamerSetOrth(const float height,
		const float aspectRatio,
		const float near,
		const float far);
	void ActiveCameraSerPers(const float fovy,
		const float aspect,
		const float near,
		const float far);



};