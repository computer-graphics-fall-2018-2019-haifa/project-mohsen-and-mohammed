#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

MeshModel::MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName) :
	modelName(modelName),
	worldTransform(glm::mat4x4(1))
{

}

MeshModel::~MeshModel()
{

}

void MeshModel::SetWorldTransformation(const glm::mat4x4& worldTransform)
{
	this->worldTransform = worldTransform;
}

const glm::mat4x4& MeshModel::GetWorldTransformation() const
{
	return worldTransform;
}

void MeshModel::SetColor(const glm::vec4& color)
{
	this->color = color;
}

const glm::vec4& MeshModel::GetColor() const
{
	return color;
}

const std::string& MeshModel::GetModelName()
{
	return modelName;
}

void MeshModel::calculateNormalPerFace() {
	for (int i=0;i<faces.size();++i) {
		glm::vec3 newNormal(0, 0, 0);
		for (int j = 0; j < 3; j++) {
			newNormal += normals.at(faces.at(i).GetNormalIndex(j));
		}
		faces.at(i).SetNormal(Utils::SwitchFromHom(Utils::Scale(glm::vec3(0.33f,0.33f,0.33f))*Utils::HomCoordinats(newNormal)));//TODO: scailing matri
	}
}