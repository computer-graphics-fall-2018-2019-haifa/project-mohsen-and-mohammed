#pragma once
#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"
#include <cmath>
#include "MeshModel.h"
/*
 * Utils class.
 * This class is consisted of static helper methods that can have many clients across the code.
 */
enum AXIS { X = 'x', Y = 'y', Z = 'z' };
enum PROJECTION { ORTH = 'O', PERS = 'P' };
class Utils
{
	static std::shared_ptr<MeshModel> cam;
public:
	static glm::vec3 Vec3fFromStream(std::istream& issLine);
	static glm::vec2 Vec2fFromStream(std::istream& issLine);
	static MeshModel LoadMeshModel(const std::string& filePath);
	
	static glm::vec3 SwitchFromHom(const glm::vec4& vector);/*transform a vector in homogeneous  coordinates to standart coordinates*/
	static glm::vec4 HomCoordinats(const glm::vec3& vector);/*transform the given vector to its homogeneous form*/

	static float dotProduct(const glm::vec3& vector1,const glm::vec3& vector2);
	static glm::vec3 crossProduct(const glm::vec3& vector1, const glm::vec3& vector2);

	static glm::mat4 ReflectAxis(AXIS x);
	//static glm::mat4& ReflectByPlane(const glm::vec3& vector1,const glm::vec3& vector2);
	static glm::mat4 Scale(const glm::vec3& scaleVec);
	static glm::mat4 Translate(const glm::vec3& translateVec);
	static glm::mat4 RotateOrigin(const float theta, const AXIS axis);
	static glm::mat4 RotateAround(const float theta, const glm::vec3& point,const glm::vec3& direction);
	static glm::mat4 ViewPortTramsform(const float left,const float right,const float buttom,const float top);
	static glm::vec3 NormalizeVector(const glm::vec3&);
	static glm::vec3 Normalize(const glm::vec3);
	static glm::vec4 Normalize(const glm::vec4&);/*this method normalize the vector in hom coordinates*/
	static float Norm(const glm::vec3);
	static float floatPresice(float x);
	static void floatPresice(glm::vec3&);
	static void floatPresice(glm::vec4&);
	static MeshModel getCameraModel();
	static float FindXLine(const glm::vec2& p1, const glm::vec2& p2, const float yy);//this method compute x coordinate of the a point with the given y coordinate in the line that
	//connect p1 and p2
	static float FindYLine(const glm::vec2& p1, const glm::vec2& p2, const float x);
	static bool DoesContain(const glm::vec2& point,const glm::vec2& v1,const glm::vec2& v2,const glm::vec2& v3);
	//this method returns true if and only if " point " is inside the trainagle v1v2v3
	static float Area(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3);//calculates area of trainale
	static float ZInterpolation(int i,int j,const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);
private:
	static std::string GetFileName(const std::string& filePath);
};