#pragma once
#include "include.h"

class Cylinder
{
	GLfloat *vertices;
	GLuint *indices;
	GLuint slices;
	GLfloat radius;
	GLfloat length;
	GLuint VBO, EBO, VAO;
	GLint vertexParams;
	GLuint textureRepeat;
	void calculateVertexNormals() const;
public:
	Cylinder();
	Cylinder(GLuint pSlices, GLfloat pRadius, GLfloat pLength, GLuint pTextureRepeat = 1);
	~Cylinder();
	void draw() const;
};
