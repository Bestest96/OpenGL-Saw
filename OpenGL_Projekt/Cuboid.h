#pragma once
#include "include.h"

class Cuboid
{
	GLfloat width;
	GLfloat length;
	GLfloat height;
	GLint vertexParams;
	GLuint textureRepeat;
	GLfloat vertices[64];
	GLuint indices[36];
	GLuint VBO, EBO, VAO;
	void calculateVertexNormals();
public:
	Cuboid(GLfloat pWidth, GLfloat pHeight, GLfloat pLength, GLuint pTextureRepeat = 1);
	~Cuboid();
	void draw() const;
};