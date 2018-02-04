#include "LightSource.h"

LightSource::~LightSource()
{
	
}

LightSource::LightSource(glm::vec3 pLightColor, glm::vec3 pLightPosition): Cuboid(0.5, 0.5, 0.5), lightColor(pLightColor), lightPosition(pLightPosition)
{
	
} 

void LightSource::draw() const
{
	Cuboid::draw();
}

glm::vec3 LightSource::getLightPosition() const
{
	return lightPosition;
}

glm::vec3 LightSource::getLightColor() const
{
	return lightColor;
}

void LightSource::setLightPosition(glm::vec3 position)
{
	lightPosition = position;
}

void LightSource::setLightColor(glm::vec3 color)
{
	lightColor = color;
}




