#include "Light.h"

Light::Light(glm::vec3 postion, glm::vec3 color, bool isParallel) :
	position(postion), color(color), parallel(isParallel)
{
}

Light::~Light()
{
}

glm::vec3 Light::getColor()const {
	return color;
}

void Light::setColor(const glm::vec3& newCol) {
	color = newCol;
}

glm::vec3 Light::getPostion()const {
	return position;
}

void Light::setPosition(const glm::vec3& newPos) {
	position = newPos;
}

bool Light::isParallel()const {
	return parallel;
}