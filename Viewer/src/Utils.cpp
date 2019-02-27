#include "Utils.h"
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#define MAX(x,y) (x>=y)?x:y
#define MIN(x,y) (x<y)?x:y
glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

MeshModel Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::ifstream ifile(filePath.c_str());

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			vertices.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vn")
		{
			normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			// Texture coordinates
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}
	return MeshModel(faces, vertices, normals, Utils::GetFileName(filePath));
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}

glm::vec3 Utils::SwitchFromHom(const glm::vec4& vector) {
	//if (vector.w != 0) {
		return glm::vec3(vector.x , vector.y, vector.z );
	//}
	/*else {
		std::cout << "invalid vector!." << std::endl;
		exit(1);
	}*/
}

glm::vec4 Utils::HomCoordinats(const glm::vec3& vector) {
	return glm::vec4(vector.x, vector.y, vector.z, 1);
}

float Utils::dotProduct(const glm::vec3& vector1, const glm::vec3& vector2) {
	return vector1.x*vector2.x + vector1.y*vector2.y + vector1.z*vector2.z;
}

glm::vec3 Utils::crossProduct(const glm::vec3& v1,const glm::vec3& v2) {
	const float x = v1.y*v2.z - v1.z*v2.y;
	const float y = -v1.x*v2.z+v1.z*v2.x;
	const float z = v1.x*v2.y-v1.y*v2.x;
	return glm::vec3(x, y, z);
}

glm::mat4 Utils::ReflectAxis(AXIS axis) {
	typedef glm::vec4 myVec;
	if (axis == X) {
		return glm::mat4(myVec(-1, 0, 0, 0), myVec(0, 1, 0, 0), myVec(0, 0, 1, 0), myVec(0, 0, 0, 1));
	}
	else if (axis == Y) {
		return glm::mat4(myVec(1, 0, 0, 0), myVec(0, -1, 0, 0), myVec(0, 0, 1, 0), myVec(0, 0, 0, 1));
	}
	else {
		return glm::mat4(myVec(1, 0, 0, 0), myVec(0, 1, 0, 0), myVec(0, 0,-1, 0), myVec(0, 0, 0, 1));
	}
}
/*
glm::mat4& Utils::ReflectByPlane(const glm::vec3& vector1,const glm::vec3& vector2) {

}
*/
glm::mat4 Utils::Scale(const glm::vec3& vector) {
	glm::vec4 row1(vector.x,0,0,0);
	glm::vec4 row2(0,vector.y,0,0);
	glm::vec4 row3(0,0,vector.z,0);
	glm::vec4 row4(0,0,0,1);
	return glm::mat4(row1, row2, row3, row4);
}

glm::mat4 Utils::Translate(const glm::vec3& vector) {
	glm::vec4 row1(1, 0, 0, vector.x);
	glm::vec4 row2(0, 1, 0, vector.y);
	glm::vec4 row3(0, 0, 1, vector.z);
	glm::vec4 row4(0, 0, 0, 1);
	return glm::mat4(row1, row2, row3, row4);
}

glm::mat4 Utils::RotateOrigin(const float theta,const AXIS around) {
	typedef  glm::vec4 myVec;
	if (around == X) {
		return glm::mat4(myVec(1, 0, 0, 0), myVec(0, cos(theta), -sin(theta), 0), myVec(0, sin(theta), cos(theta), 0), myVec(0, 0, 0, 1));
	}
	else if (around == Y) {
		return glm::mat4(myVec(cos(theta), 0, -sin(theta), 0), myVec(0, 1, 0, 0), myVec(sin(theta), 0, cos(theta), 0), myVec(0, 0, 0, 1));
	}
	else {
		return glm::mat4(myVec(cos(theta), -sin(theta), 0, 0), myVec(sin(theta), cos(theta), 0, 0), myVec(0, 0, 1, 0), myVec(0, 0, 0, 1));
	}
}

glm::mat4 Utils::RotateAround(const float theta, const glm::vec3& point,const glm::vec3& direction) {
	const float x= direction.x-point.x;
	const float y = direction.y - point.y;
	const float z = direction.z - point.z;
	const float thetaX=asinf(z/sqrtf(x*x+y*y+z*z));
	const float thetaY= acosf(y/sqrtf(x*x+y*y));
	//const float thetaZ=theta;
	glm::vec3& minusPoint=point*-1.0f;
	glm::mat4 moveToOrigin = Utils::RotateOrigin(-thetaY, Y)*Utils::RotateOrigin(-thetaX, X)*Utils::Translate(minusPoint);
	glm::mat4 moveToOriginInverse = Utils::Translate(point)*Utils::RotateOrigin(thetaX,X)*Utils::RotateOrigin(thetaY,Y);
	return moveToOriginInverse* Utils::RotateOrigin(theta, Z)*moveToOrigin;
}

glm::vec3 Utils::NormalizeVector(const glm::vec3& vector1) {
	//std::cout << "x " << vector1.x <<" y "<< vector1.y << " z "<< vector1.z<<std::endl;
	//glm::vec3 vector = Utils::SwitchFromHom(vector1);
	const float normal = 1/sqrtf(vector1.x*vector1.x + vector1.y*vector1.y+ vector1.z*vector1.z);
	//std::cout << "normal " << 1 / normal << std::endl;
	return glm::vec3(normal,normal,normal)*vector1;
}

glm::mat4 Utils::ViewPortTramsform(const float left, const float right, const float buttom, const float top) {
	glm::vec4 row1((right-left)/(2.0f),0,0,(left+right)/(2.0f));
	glm::vec4 row2(0,(top-buttom)/(2.0f),0,(top+buttom)/(2.0f));
	glm::vec4 row3(0,0,0.50f,0.50f);
	glm::vec4 row4(0,0,0,1);
	return glm::mat4(row1, row2, row3, row4);
}

glm::vec3 Utils::Normalize(const glm::vec3 vector) {
	float norm =1.0f/ sqrtf(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z);
	return Utils::HomCoordinats(vector)*norm;
}

float Utils::Norm(const glm::vec3 vector) {
	return sqrtf(vector.x*vector.x + vector.y * vector.y + vector.z * vector.z);
}

float Utils::floatPresice(float x) {
	return (fabs(x - (int)x) <= 1.0f/360.0f) ? (int)x : x;
}

void Utils::floatPresice(glm::vec3& vec) {
	vec.x = floatPresice(vec.x);
	vec.y = floatPresice(vec.y);
	vec.z = floatPresice(vec.z);
}

void Utils::floatPresice(glm::vec4& vec) {
	vec.x = floatPresice(vec.x);
	vec.y = floatPresice(vec.y);
	vec.z = floatPresice(vec.z);
	vec.w = floatPresice(vec.w);
}

glm::vec4 Utils::Normalize(const glm::vec4& v) {
	const float d = 1.0f / sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
	return glm::vec4(v.x*d ,v.y*d , v.z*d, 1);
}

MeshModel Utils::getCameraModel() {
	MeshModel temp = Utils::LoadMeshModel("C:\\Users\\USER\\Documents\\GitHub\\project-mohsen-and-mohammed\\Data\\camera.obj");
	temp.TransformVertices(Utils::Scale(glm::vec3(1, 1, -1)));
	return  temp;
}

float Utils::FindXLine(const glm::vec2& p1, const glm::vec2& p2, const float yy) {
	if (p1.y == p2.y) {  throw 0; }
	if (p1.x == p2.x) { return p1.x; }
	return /*0;*/ (-1 * (p1.y - yy)*(p1.x - p2.x) / (p1.y - p2.y)) + p1.x;
}

float Utils::FindYLine(const glm::vec2& p1, const glm::vec2& p2, const float x) {
	if (p1.x == p2.x) { throw 0; }
	if (p1.y == p2.y) { return p1.y; }
	return /*0;*/ (-1 * (p1.x - x)*(p1.y - p2.y) / (p1.x - p2.x)) + p1.y;
}

float Utils::Area(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3) {
	return fabs((v1.x*(v2.y - v3.y) - v1.y * (v2.x - v3.x) +(v2.x*v3.y - v2.y*v3.x)) / 2.0f);
}

float Utils::Area3(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
	const glm::vec3 temp1 = v2 - v1;
	const glm::vec3 temp2 = v3 - v1;
	const glm::vec3 cross = Utils::crossProduct(temp1, temp2);
	return 0.5*(sqrtf(cross.x*cross.x + cross.y*cross.y + cross.z*cross.z));
}

bool Utils::DoesContain(const glm::vec2& p, const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3) {
	float l1 = (p.x - v1.x)*(v3.y - v1.y) - (v3.x - v1.x)*(p.y - v1.y),
		l2 = (p.x - v2.x)*(v1.y - v2.y) - (v1.x - v2.x)*(p.y - v2.y),
		l3 = (p.x - v3.x)*(v2.y - v3.y) - (v2.x - v3.x)*(p.y - v3.y);
	return (l1 > 0 && l2 > 0 && l3 > 0) || (l1 < 0 && l2 < 0 && l3 < 0);
}

float Utils::ZInterpolation(int i, int j, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
	const float area = Utils::Area(glm::vec2(v1.x, v1.y), glm::vec2(v2.x, v2.y), glm::vec2(v3.x, v3.y));
	const float area1 = Utils::Area(glm::vec2(v2.x, v2.y), glm::vec2(v3.x, v3.y), glm::vec2(i, j));
	const float area2 = Utils::Area(glm::vec2(v1.x, v1.y), glm::vec2(i, j), glm::vec2(v3.x, v3.y));
	const float area3 = Utils::Area(glm::vec2(i, j), glm::vec2(v2.x, v2.y), glm::vec2(v1.x, v1.y));
	return ((area1)*v1.z + (area2)*v2.z + (area3)*v3.z)/area;
}

float Utils::ZInterpolation(int i, int j, const glm::vec3& v1, const glm::vec3& v2) {
	if (v1.x == v2.x && v1.y == v2.y) {
		return std::numeric_limits<float>::max();
	}
	if (v1.x > v2.x) {
		const float t = (i - v1.x) / (v2.x - v1.x);
		return t * v2.z + (1 - t)*v1.z;
	}
	else if (v2.x > v1.x) {
		const float t = (i - v2.x) / (v2.x - v2.x);
		return t * v1.z + (1 - t)*v2.z;
	}
	else if (v1.y>v2.y) {
		const float t = (i - v1.y) / (v2.y-v1.y);
		return t * v2.z + (1 - t)*v1.z;
	}
	else/*(v2.y > v1.y)*/ {
		const float t = (i - v2.y) / (v1.y - v2.y);
		return t * v1.z + (1 - t)*v2.z;
	}
}

glm::vec3 Utils::InterpolateNormal(const glm::vec3& p1,const glm::vec3& p2,const glm::vec3& p3,const glm::vec3& q,
	const glm::vec3& n1,const glm::vec3& n2,const glm::vec3& n3) {
	//we assume that !(p1==p2 || p2==p3 || p1==p3) 
	const float area = Utils::Area3(p1, p2, p3);
	const float area1 = Utils::Area3(p2, p3, q);
	const float area2 = Utils::Area3(p1, p3, q);
	const float area3 = Utils::Area3(p1, p2, q);
	const float a1 = area1 / area, a2 = area2 / area, a3 = area3 / area;
	//if (area1 > area || area2 > area || area3 > area)std::cout << "WTF!" << std::endl;
	const float newZ = a1 * n1.z + a2 * n2.z + a3 * n3.z;
	const float newY = a1 * n1.y + a2 * n2.y + a3 * n3.y;
	const float newX = a1 * n1.x + a2 * n2.x + a3 * n3.x;
	return glm::vec3(newX, newY, newZ);
}
