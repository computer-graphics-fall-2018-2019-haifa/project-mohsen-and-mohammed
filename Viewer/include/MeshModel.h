#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Face.h"

/*
 * MeshModel class.
 * This class represents a mesh model (with faces and normals informations).
 * You can use Utils::LoadMeshModel to create instances of this class from .obj files.
 */
class MeshModel
{
private:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> vertixNormals;

	glm::mat4x4 worldTransform;
	glm::mat4x4 modelTransform;

	glm::vec4 color;
	std::string modelName;

	float modelThetaX;
	float modelThetaY;
	float modelThetaZ;

	float worldThetaX;
	float worldThetaY;
	float worldThetaZ;

	glm::vec3 modelScale;

	glm::vec3 modelTranslate;
	glm::vec3 worldTranslate;
	
public:
	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName = "");
	virtual ~MeshModel();

	void SetWorldTransformation(const glm::mat4x4& worldTransform);
	const glm::mat4x4& GetWorldTransformation() const;

	void SetModelTransformation(const glm::mat4x4& modelTransform);
	const glm::mat4x4& GetModelTransformation() const;

	const glm::vec4& GetColor() const;
	void SetColor(const glm::vec4& color);
	
	const Face& getFace(int index)const;

	const std::string& GetModelName();

	void calculateNormalPerFace();
	
	const std::vector<glm::vec3> GetVertices(const int faceIndex)const;

	const std::vector<glm::vec3> GetNormals(const int faceIndex) const;

	float GetFaceArea(const int faceIndex) const;

	glm::vec3 GetFaceCenter(const int faceIndex) const;
	glm::vec3 GetFaceNormalDirection(const int faceIndex) const;
	int GetVerticesCount()const;
	int GetFaceCount()const;
	bool DoesFaceContainVertix(const int faceIndex, const int vertixIndex) const;
	void printV()const;

	float getMaxX()const;
	float getMaxY()const;
	float getMaxZ()const;
	float getMinX()const;
	float getMinY()const;
	float getMinZ()const;

	glm::vec3 getVertix(const int vetixIndex)const;
	glm::vec3 getVertixNormal(const int vertixIndex)const;

	float getModelThetaX()const;
	float getModelThetyaY()const;
	float getModelThetaZ()const;

	float getWorldThetaX()const;
	float getWorldThetaY()const;
	float getWorldThetaZ()const;

	glm::vec3 getModelScale()const;
	glm::vec3 getModelTranslate()const;
	glm::vec3 getWorldTranslate()const;

	void setModelThetaX(const float newTheta);
	void setModelThetaY(const float newTheta);
	void setModelThetaZ(const float newTheta);

	void setWorldThetaX(const float newTheta);
	void setWorldThetaY(const float newTheta);
	void setWorldThetaZ(const float newTheta);

	void setModelScale(const glm::vec3& newScale);

	void setModelTranslate(const glm::vec3& newTranslate);

	void setWorldTranslate(const glm::vec3& newTranslate);

	glm::vec3 CalculateInModelFrame(const glm::vec3&)const;
	glm::vec3 CalculateInModelFrameDirection(const glm::vec3&)const;

	float getMinXAfterTranformX(const glm::mat4& Transform)const;
	float getMaxAfterTransformX(const glm::mat4& Tranform)const;

	void TransformVertices(const glm::mat3&);
};
