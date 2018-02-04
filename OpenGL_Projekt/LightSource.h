#pragma once
#include "include.h"
#include "Cuboid.h"

class LightSource: public Cuboid
{
	glm::vec3 lightColor;
	glm::vec3 lightPosition;
public:
	LightSource(glm::vec3 pLightColor = glm::vec3(1.0), glm::vec3 pLightPosition = glm::vec3(0.0, 10.0, -20.0));
	~LightSource();
	glm::vec3 getLightPosition() const;
	void draw() const;
	glm::vec3 getLightColor() const;
	void setLightPosition(glm::vec3 position);
	void setLightColor(glm::vec3 color);
};
