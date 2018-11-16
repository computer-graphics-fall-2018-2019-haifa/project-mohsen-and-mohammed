#pragma once
#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"
#include <cmath>

/*
 * Utils class.
 * This class is consisted of static helper methods that can have many clients across the code.
 */
enum AXIS { X = 'x', Y = 'y', Z = 'z' };

class Utils
{
public:
	static glm::vec3 Vec3fFromStream(std::istream& issLine);
	static glm::vec2 Vec2fFromStream(std::istream& issLine);
	static MeshModel LoadMeshModel(const std::string& filePath);
	
	static glm::vec3 SwitchFromHom(const glm::vec4& vector);/*transform a vector in homogeneous  coordinates to standart coordinates*/
	static glm::vec4 HomCoordinats(const glm::vec3& vector);/*transform the given vector to its homogeneous form*/

	static float dotProduct(const glm::vec3& vector1,const glm::vec3& vector2);
	static glm::vec3& crossProduct(const glm::vec3& vector1, const glm::vec3& vector2);

	static glm::mat4& ReflectAxis(AXIS x);
	//static glm::mat4& ReflectByPlane(const glm::vec3& vector1,const glm::vec3& vector2);
	static glm::mat4& Scale(const glm::vec3& scaleVec);
	static glm::mat4& Translate(const glm::vec3& translateVec);
	static glm::mat4& RotateOrigin(const float theta, const AXIS axis);
	static glm::mat4& RotateAround(const float theta, const glm::vec3& point,const glm::vec3& direction);
	
	
private:
	static std::string GetFileName(const std::string& filePath);
};