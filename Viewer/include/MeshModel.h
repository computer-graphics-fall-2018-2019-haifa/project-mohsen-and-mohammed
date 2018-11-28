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
	glm::mat4x4 worldTransform;
	glm::vec4 color;
	std::string modelName;
	float getMinXAfterTranformX(const glm::mat4& Transform)const;
	float getMaxAfterTransformX(const glm::mat4& Tranform)const;
public:
	MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName = "");
	virtual ~MeshModel();

	void SetWorldTransformation(const glm::mat4x4& worldTransform);
	const glm::mat4x4& GetWorldTransformation() const;

	const glm::vec4& GetColor() const;
	void SetColor(const glm::vec4& color);

	const std::string& GetModelName();

	void calculateNormalPerFace();

	const std::vector<glm::vec3> GetVertices(const int faceIndex)const;

	const std::vector<glm::vec3> GetNormals(const int faceIndex) const;

	int GetVerticesCount()const;

	void printV()const;

	float getMaxX()const;
	float getMaxY()const;
	float getMaxZ()const;
	float getMinX()const;
	float getMinY()const;
	float getMinZ()const;
	// Add more methods/functionality as needed...
};
