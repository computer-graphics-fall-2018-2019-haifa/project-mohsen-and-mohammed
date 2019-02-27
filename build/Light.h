#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
class Light
{
	glm::vec3 position;
	glm::vec3 color;
	bool parallel;
public:
	Light(glm::vec3 postion, glm::vec3 color, bool isParallel);
	~Light();

	glm::vec3 getPostion()const;
	void setPosition(const glm::vec3& newPostion);

	glm::vec3 getColor()const;
	void setColor(const glm::vec3& newColor);

	bool isParallel()const;
};