#pragma once
#include "include.h"

class HalfCylinder
{
	GLfloat *vertices;
	GLuint *indices;
	GLuint slices;
	GLfloat radius;
	GLfloat length;
	GLint vertexParams;
	GLuint textureRepeat;
	GLuint VBO, EBO, VAO;
	void calculateVertexNormals() const;
public:
	HalfCylinder();
	HalfCylinder(GLuint pSlices, GLfloat pRadius, GLfloat pLength, GLuint pTextureRepeat = 1);
	~HalfCylinder();
	void draw() const;
};