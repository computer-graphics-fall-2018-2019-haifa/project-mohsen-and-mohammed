#include "Face.h"
#include <iostream>
Face::Face(std::istream& issLine)
{
	vertexIndices =  { 0, 0, 0 };
	normalIndices =  { 0, 0, 0 };
	textureIndices = { 0, 0, 0 };

	char c;
	for (int i = 0; i < 3; i++)
	{
		issLine >> std::ws >> vertexIndices.at(i) >> std::ws;

		if (issLine.peek() != '/')
		{
			continue;
		}

		issLine >> c >> std::ws;

		if (issLine.peek() == '/')
		{
			issLine >> c >> std::ws >> normalIndices.at(i);
			continue;
		}
		else
		{
			issLine >> textureIndices.at(i);
		}

		if (issLine.peek() != '/')
		{
			continue;
		}

		issLine >> c >> normalIndices.at(i);
	}

}

Face::~Face()
{

}

const int Face::GetVertexIndex(int index) const
{
	return vertexIndices[index];
}

const int Face::GetNormalIndex(int index) const
{
	return normalIndices[index];
}

const int Face::GetTextureIndex(int index) const
{
	return textureIndices[index];
}

void Face::SetNormal(const glm::vec3& normal) {
	faceNormal = normal;
}

const glm::vec3 Face::GetNormal() const{
	return this->faceNormal;
}

bool Face::ContainVertix(const int vertix) const {
	//std::cout << vertexIndices.at(0) << std::endl;
	return (vertexIndices.at(0) == vertix || vertexIndices.at(1) == vertix || vertexIndices.at(2) == vertix);
}

