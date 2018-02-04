#pragma once
#include "include.h"

class Saw
{
	GLfloat *vertices;
	GLuint *indices;
	GLuint spikeCount;
	GLfloat width;
	GLfloat height;
	GLfloat length;
	GLuint VBO, EBO, VAO;
	GLint vertexParams;
	GLuint textureRepeat;
	void calculateVertexNormals() const;
public:
	Saw();
	Saw(GLuint pSpikeCount, GLfloat pWidth, GLfloat pHeight, GLfloat pLength, GLuint pTextureRepeat = 1);
	~Saw();
	void draw() const;
};
