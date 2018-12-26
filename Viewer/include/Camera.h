#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "MeshModel.h"
#include "Face.h"
#include "Utils.h"

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
	float _aspectRatiop;
	float _aspectRatioo;
	float _fovy;
	float _nearo;
	float _faro;
	float _nearp;
	float _farp;

	float rotateX;
	float rotateY;
	float rotateZ;

	glm::vec4 eye;
	glm::vec4 at;
	glm::vec4 y;



	glm::mat4 calculateOrthProjectionMatrix(const float height,
		const float aspectRatio,
		const float _near,
		const float _far)const ;
	glm::mat4 calculatePersProjetionMatrix(const float fovy,
		const float aspect,
		const float _near,
		const float _far)const;
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
	
	bool CanBeSeen(const std::vector<glm::vec4>&) const;
	const glm::mat4& GetInverseViewTranform()const;
	const glm::mat4& GetProjectionTransform()const;

	float getXRotate()const;
	void setXRotate(const float t);
	float getYRotate()const;
	void setYRotate(const float t);
	float getZRotate()const;
	void setZRotate(const float t);

	glm::vec3 getEye()const;
	void setEye(const glm::vec3& vec);
	glm::vec3 getAt()const;
	void setAt(const glm::vec3& vec);
	glm::vec3 getY()const;
	void setY(const glm::vec3& vec);

	float getFovy()const;
	float getNearO()const;
	float getFarO()const;
	float getNearP()const;
	float getFarP()const;
	float getAspectO()const;
	float getAspectP()const;
	float getHeight()const;
	float getZoom()const;

};

