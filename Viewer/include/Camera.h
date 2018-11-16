#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "MeshModel.h"
#include "Face.h"


/*
 * Camera class. This class takes care of all the camera transformations and manipulations.
 *
 * Implementation suggestion:
 * --------------------------
 * Make the Camera class be a subclass of MeshModel, so you can easily and elegantly render 
 * the cameras you have added to the scene.
 */
class Camera/*: public MeshModel*/
{
private:
	glm::mat4x4 viewTransformation;
	glm::mat4x4 InverseViewTransformation;
	glm::mat4x4 projectionTransformation;
	PROJECTION pro;
	float _zoom;
	float _height;
	float _aspectRatio;
	float _fovy;
	float _near;
	float _far;

	glm::mat4 calculateOrthProjectionMatrix(const float height,
		const float aspectRatio,
		const float near,
		const float far)const ;
	glm::mat4 calculatePersProjetionMatrix(const float fovy,
		const float aspect,
		const float near,
		const float far)const;
	bool AUX_CanBeSeen(const glm::vec4&) const;
public:
	Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up);
	~Camera();

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	void SetOrthographicProjection(
		const float height,
		const float aspectRatio,
		const float near,
		const float far);

	void SetPerspectiveProjection(
		const float fovy,
		const float aspect,
		const float near,
		const float far);

	void SetZoom(const float zoom);
	
	bool CanBeSeen(const std::vector<glm::vec3>&) const;

	// Add more methods/functionality as needed...

};


enum PROJECTION {ORTH='O',PERS='P'};