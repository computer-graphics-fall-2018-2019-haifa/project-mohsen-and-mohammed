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
	vertixNormals.resize(vertices.size(), glm::vec3(0, 0, 0));
	for (int i = 0; i < faces.size(); i++) {
		const std::vector<glm::vec3> faceNormals = MeshModel::GetNormals(i);
		for (int j = 0; j < 3; j++) {
			vertixNormals.at(faces.at(i).GetVertexIndex(j)-1) += faceNormals.at(j);
		}
	}
	for (int i = 0; i < vertixNormals.size(); i++) {
		vertixNormals.at(i) = Utils::Normalize(vertixNormals.at(i)) / 5.0f;
	}
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


float MeshModel::GetFaceArea(const int faceIndex) const {
	const std::vector < glm::vec3 > points= MeshModel::GetVertices(faceIndex);
	const glm::vec3 vector1 = points.at(0) - points.at(1);
	const glm::vec3 vector2 = points.at(0) - points.at(2);
	const float t = Utils::dotProduct(Utils::Normalize(vector1), Utils::Normalize(vector2));
	if (t >= -1 && t <= 1) {
		const float theta = acosf(t);
		return 0.5f*Utils::Norm(vector1)*Utils::Norm(vector2)*sinf(theta);
	}
	else {
		exit(1);
	}
	
}

glm::vec3 MeshModel::GetFaceCenter(const int faceIndex) const {
	glm::vec3 center(0, 0, 0);
	const std::vector<glm::vec3> vertices = MeshModel::GetVertices(faceIndex);
	for (int i = 0; i < 3; i++) {
		center += vertices.at(i);
	}
	return center * 0.33f;
}

int MeshModel::GetVerticesCount()const {
	return vertices.size();
}

int MeshModel::GetFaceCount()const {
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

bool MeshModel::DoesFaceContainVertix(const int faceIndex, const int vertixIndex) const {
	return faces.at(faceIndex).ContainVertix(vertixIndex);
}

glm::vec3 MeshModel::GetFaceNormalDirection(const int faceIndex) const {
	if (faceIndex < 0 || faceIndex >= faces.size()) {
		std::cout << std::endl << "ASDASDASDASDASDASDafsdgasdfg" << std::endl;
		exit(1);
	}
	std::vector<glm::vec3> vertices = MeshModel::GetVertices(faceIndex);
	glm::vec3 vector1 = vertices.at(0) - vertices.at(1);
	glm::vec3 vector2 = vertices.at(0) - vertices.at(2);
	if (vector1.x <= vector2.x) {
		return Utils::Normalize(Utils::crossProduct(vector1, vector2)) / 5.0f;
	}
	else {
		return Utils::Normalize(Utils::crossProduct(vector2, vector1)) / 5.0f;
	}
}

glm::vec3 MeshModel::getVertix(const int index)const {
	return vertices.at(index);
}

glm::vec3 MeshModel::getVertixNormal(const int index)const {
	return vertixNormals.at(index);
}