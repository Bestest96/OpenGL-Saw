#pragma once
#include "include.h"

class Skybox
{
	GLfloat vertices[108];
	GLuint VBO, VAO;
	GLuint skyboxTexture;
	GLuint loadCubemap();
public:
	Skybox();
	~Skybox();
	void draw() const;
};
