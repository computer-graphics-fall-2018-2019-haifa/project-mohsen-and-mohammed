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
	this->faces = faces;
	this->vertices = vertices;
	this->normals = normals;
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

void MeshModel::calculateNormalPerFace()  {
	for (int i=0;i<faces.size();++i) {
		glm::vec3 newNormal(0, 0, 0);
		for (int j = 0; j < 3; j++) {
			newNormal += normals.at(faces.at(i).GetNormalIndex(j));
		}
		faces.at(i).SetNormal(Utils::SwitchFromHom(Utils::Scale(glm::vec3(0.33f,0.33f,0.33f))*Utils::HomCoordinats(newNormal)));
	}
}
const std::vector<glm::vec3> MeshModel::GetVertices(const int faceIndex)const {
	std::vector<glm::vec3> myVertices;
	for (int i = 0; i < 3; i++) {
		//std::cout << "Vertex Index:" << vertices.at(faces.at(faceIndex).GetVertexIndex(i) - 1).x << std::endl;
		myVertices.push_back(vertices.at(faces.at(faceIndex).GetVertexIndex(i)-1));
	}
	return myVertices;
}

const std::vector<glm::vec3> MeshModel::GetNormals(const int faceIndex) const {
	std::vector<glm::vec3> myNormals;
	for (int i = 0; i < 3; i++) {
		myNormals.push_back(normals.at(faces.at(faceIndex).GetNormalIndex(i)-1));
	}
	return myNormals;
}

int MeshModel::GetVerticesCount()const {
	return faces.size();
}


void MeshModel::printV() const{
	static int h = 0;
	if (h > 0)return;
	for (int i = 0; i < vertices.size(); i++) {
		std::cout << i << "-x:" << vertices.at(i).x << ",y:" << vertices.at(i).y << " z:" << vertices.at(i).z << std::endl;
	}
}

float MeshModel::getMinXAfterTranformX(const glm::mat4& tranfrom)const {
	/*we assume tha tthe mesh model have at least one vertix*/
	float min = (tranfrom* Utils::HomCoordinats(vertices.at(0))).x;
	for (int i = 0; i < vertices.size(); i++) {
		float temp = (tranfrom* Utils::HomCoordinats(vertices.at(i))).x;
		//std::cout << "Dick: " << temp << std::endl;
		if (min >= temp)
			min = temp;
	}
	return min;
}

float MeshModel::getMaxAfterTransformX(const glm::mat4& transform)const{
	/*we assume tha tthe mesh model have at least one vertix*/
	glm::mat4 reverse= Utils::Scale(glm::vec3(-1, -1, -1));
	return -1.0f*MeshModel::getMinXAfterTranformX(reverse*transform);
}

float MeshModel::getMaxX()const {
	return getMaxAfterTransformX(glm::mat4(1));
}
float MeshModel::getMaxY()const {
	glm::vec4 row1(0,1,0,0);
	glm::vec4 row2(1,0,0,0);
	glm::vec4 row3(0,0,1,0);
	glm::vec4 row4(0,0,0,1);
	return getMaxAfterTransformX(glm::mat4(row1,row2,row3,row4));
}
float MeshModel::getMaxZ()const {
	glm::vec4 row1(0, 0, 1, 0);
	glm::vec4 row2(0, 1, 0, 0);
	glm::vec4 row3(1, 0, 0, 0);
	glm::vec4 row4(0, 0, 0, 1);
	return getMaxAfterTransformX(glm::mat4(row1, row2, row3, row4));
}
float MeshModel::getMinX()const {
	return getMinXAfterTranformX(glm::mat4(1));
}
float MeshModel::getMinY()const {
	glm::vec4 row1(0, 1, 0, 0);
	glm::vec4 row2(1, 0, 0, 0);
	glm::vec4 row3(0, 0, 1, 0);
	glm::vec4 row4(0, 0, 0, 1);
	return getMinXAfterTranformX(glm::mat4(row1, row2, row3, row4));
}
float MeshModel::getMinZ()const {
	glm::vec4 row1(0, 0, 1, 0);
	glm::vec4 row2(0, 1, 0, 0);
	glm::vec4 row3(1, 0, 0, 0);
	glm::vec4 row4(0, 0, 0, 1);
	return getMinXAfterTranformX(glm::mat4(row1, row2, row3, row4));
}