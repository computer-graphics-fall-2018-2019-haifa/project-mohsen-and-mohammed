#include "MeshModel.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

MeshModel::MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName) :
	modelName(modelName),
	worldTransform(glm::mat4x4(1)),modelTransform(glm::mat4(1))
	, modelThetaX(0), modelThetaY(0), modelThetaZ(0), worldThetaX(0), worldThetaY(0), worldThetaZ(0)
	,modelScale(glm::vec3(1,1,1)), 
	modelTranslate(glm::vec3(0,0,0)),worldTranslate(glm::vec3(0,0,0))
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
	return (int)vertices.size();
}

int MeshModel::GetFaceCount()const {
	return (int)faces.size();
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

float MeshModel::getModelThetaX()const {
	return modelThetaX;
}
float MeshModel::getModelThetyaY()const {
	return modelThetaY;
}
float MeshModel::getModelThetaZ()const {
	return modelThetaZ;
}
glm::vec3 MeshModel::getModelScale()const {
	return modelScale;
}
glm::vec3 MeshModel::getModelTranslate()const {
	return modelTranslate;
}
void MeshModel::setModelThetaX(const float theta) {
	this->modelThetaX = theta;
}
void MeshModel::setModelThetaY(const float theta) {
	this->modelThetaY = theta;
}
void MeshModel::setModelThetaZ(const float theta) {
	this->modelThetaZ = theta;
}
void MeshModel::setModelScale(const glm::vec3& newScale) {
	this->modelScale = newScale;
}
void MeshModel::setModelTranslate(const glm::vec3& newTranslate) {
	this->modelTranslate = newTranslate;
}
glm::vec3 MeshModel::getWorldTranslate()const {
	return worldTranslate;
}
void MeshModel::setWorldTranslate(const glm::vec3& newTranslate) {
	worldTranslate = newTranslate;
}
glm::vec3 MeshModel::CalculateInModelFrame(const glm::vec3& vector)const {
	//the standart basis of R3
	const glm::vec3 v1(1, 0, 0), v2(0, 1, 0), v3(0, 0, 1);
	//the origin of model's frame
	const glm::vec4 O = glm::transpose(worldTransform) * glm::vec4(0, 0, 0, 1);
//	std::cout << O.x << " " << O.y << " " << O.z << " " << O.w << "  O" << std::endl;
	//the basis of model frame
	glm::vec3 u1 = CalculateInModelFrameDirection(v1);glm::vec3 u2 = CalculateInModelFrameDirection(v2);
	glm::vec3 u3 = CalculateInModelFrameDirection(v3);
	//glm::vec4 u1 = glm::transpose(worldTransform )* v1-O, u2 = glm::transpose(worldTransform) * v2-O, u3 = glm::transpose(worldTransform) * v3-O;
	u1 = Utils::Normalize(u1); u2 = Utils::Normalize(u2); u3 = Utils::Normalize(u3);
	/*std::cout << " " << u1.x << " " << u1.y << " " << u1.z << " u1" << u1.w<<std::endl;
	std::cout << " " << u2.x << " " << u2.y << " " << u2.z << " u2" << std::endl;
	std::cout << " " << u3.x << " " << u3.y << " " << u3.z << " u3" << std::endl;*/
	const glm::mat4x4 transformationMatrixTranspose(u1.x, u1.y, u1.z,0.0f,
										 u2.x, u2.y, u2.z,0.0f,
										 u3.x, u3.y, u3.z,0.0f,
											0.0f,0.0f,0.0f,1.0f);
	return Utils::SwitchFromHom(glm::transpose(Utils::Translate(Utils::SwitchFromHom(O)))*transformationMatrixTranspose*Utils::HomCoordinats(vector));
}
glm::vec3 MeshModel::CalculateInModelFrameDirection(const glm::vec3& vector)const {
	//the standart basis of R3
	const glm::vec4 v1(1, 0, 0, 1), v2(0, 1, 0, 1), v3(0, 0, 1, 1);
	//the basis of model frame
	 glm::vec4 u1 = glm::transpose(worldTransform) * v1, u2 = glm::transpose(worldTransform) * v2, u3 = glm::transpose(worldTransform) * v3;
	 u1 = Utils::Normalize(u1); u2 = Utils::Normalize(u2); u3 = Utils::Normalize(u3);
	 //the origin of model's frame
	const glm::vec4 O = glm::transpose(worldTransform) * glm::vec4(0, 0, 0, 1);
	const glm::mat3 transformationMatrixTranspose(u1.x, u1.y, u1.z,
		u2.x, u2.y, u2.z, 
		u3.x, u3.y, u3.z);
	return transformationMatrixTranspose * vector;
}
float MeshModel::getWorldThetaX()const {
	return worldThetaX;
}
float MeshModel::getWorldThetaY()const {
	return worldThetaY;
}
float MeshModel::getWorldThetaZ()const {
	return worldThetaZ;
}
void MeshModel::setWorldThetaX(const float theta) {
	worldThetaX = theta;
}
void MeshModel::setWorldThetaY(const float theta) {
	worldThetaY = theta;
}
void MeshModel::setWorldThetaZ(const float theta) {
	worldThetaZ = theta;
}
void MeshModel::SetModelTransformation(const glm::mat4x4& mat) {
	this->modelTransform = mat;
}
const glm::mat4x4& MeshModel::GetModelTransformation()const {
	return this->modelTransform;
}