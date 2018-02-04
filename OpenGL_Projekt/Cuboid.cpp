#include "Cuboid.h"

Cuboid::~Cuboid()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

Cuboid::Cuboid(GLfloat pWidth, GLfloat pHeight, GLfloat pLength, GLuint pTextureRepeat): width(pWidth), length(pLength), height(pHeight), vertexParams(8), textureRepeat(pTextureRepeat),
	vertices {
	-width / 2, 0.0f, 0.0f, textureRepeat * 1.0f, textureRepeat * 0.0f, 0.0f, 0.0f, 0.0f,
	width / 2, 0.0f, 0.0f, textureRepeat * 0.0f, textureRepeat * 0.0f, 0.0f, 0.0f, 0.0f,
	-width / 2, 0.0f, length, textureRepeat * 1.0f, textureRepeat * 1.0f, 0.0f, 0.0f, 0.0f,
	width / 2, 0.0f, length, textureRepeat * 0.0f, textureRepeat * 1.0f, 0.0f, 0.0f, 0.0f,
	-width / 2, -height, 0.0f, textureRepeat * 2.0f, textureRepeat * 1.0f, 0.0f, 0.0f, 0.0f,
	width / 2, -height, 0.0f, textureRepeat * 1.0f, textureRepeat * 1.0f, 0.0f, 0.0f, 0.0f,
	-width / 2, -height, length, textureRepeat * 2.0f, textureRepeat * 2.0f, 0.0f, 0.0f, 0.0f,
	width / 2, -height, length, textureRepeat * 1.0f, textureRepeat * 2.0f, 0.0f, 0.0f, 0.0
	},
	indices {
	0, 1, 2,
	1, 2, 3,
	4, 5, 6,
	5, 6, 7,
	0, 4, 5,
	0, 1, 5,
	1, 3, 5,
	3, 7, 5,
	2, 6, 7,
	2, 3, 7,
	0, 2, 4,
	4, 6, 2
	}
{
	calculateVertexNormals();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 64 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexParams * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexParams * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertexParams * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0); 
}

void Cuboid::calculateVertexNormals()
{
	const int vertexNormalsCount = 8;
	glm::vec3 *vertexNormals = new glm::vec3[vertexNormalsCount];
	const int faceNormalsCount = 12;
	for (int i = 0; i < faceNormalsCount; ++i)
	{
		GLuint p0 = indices[3 * i];
		GLuint p1 = indices[3 * i + 1];
		GLuint p2 = indices[3 * i + 2];
		glm::vec3 points[3];
		points[0] = { vertices[vertexParams * p0], vertices[vertexParams * p0 + 1], vertices[vertexParams * p0 + 2] };
		points[1] = { vertices[vertexParams * p1], vertices[vertexParams * p1 + 1], vertices[vertexParams * p1 + 2] };
		points[2] = { vertices[vertexParams * p2], vertices[vertexParams * p2 + 1], vertices[vertexParams * p2 + 2] };
		glm::vec3 U = points[1] - points[0];
		glm::vec3 V = points[2] - points[0];
		glm::vec3 faceNormal = glm::normalize(glm::cross(U, V));
		glm::vec3 insidePoint = { 0.0, - height / 2, length / 2 };
		if (glm::dot(faceNormal, insidePoint - points[0]) > 0)
			faceNormal *= -1;
		vertexNormals[p0] += faceNormal;
		vertexNormals[p1] += faceNormal;
		vertexNormals[p2] += faceNormal;
	}
	for (int i = 0; i < vertexNormalsCount; ++i)
	{
		vertexNormals[i] = glm::normalize(vertexNormals[i]);
		vertices[vertexParams * i + 5] = vertexNormals[i].x;
		vertices[vertexParams * i + 6] = vertexNormals[i].y;
		vertices[vertexParams * i + 7] = vertexNormals[i].z;
	}
	delete[] vertexNormals;
}

void Cuboid::draw() const
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

