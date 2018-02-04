#include "HalfCylinder.h"

HalfCylinder::~HalfCylinder()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	delete[] vertices;
	delete[] indices;
}

HalfCylinder::HalfCylinder() : HalfCylinder(30, 1.0, 5.0)
{}


HalfCylinder::HalfCylinder(GLuint pSlices, GLfloat pRadius, GLfloat pLength, GLuint pTextureRepeat) : slices(pSlices), radius(pRadius), length(pLength), vertexParams(8), textureRepeat(pTextureRepeat)
{
	const int vertexSize = vertexParams * (2 * (slices + 2));
	vertices = new GLfloat[vertexSize];
	vertices[0] = 0.0f;
	vertices[1] = 0.0f;
	vertices[2] = 0.0f;
	vertices[3] = textureRepeat * 0.5f;
	vertices[4] = textureRepeat * 0.5f;
	vertices[vertexSize / 2] = 0.0f;
	vertices[vertexSize / 2 + 1] = 0.0f;
	vertices[vertexSize / 2 + 2] = length;
	vertices[vertexSize / 2 + 3] = textureRepeat * 1.5f;
	vertices[vertexSize / 2 + 4] = textureRepeat * 1.5f;
	for (unsigned int i = 0; i < slices + 1; ++i)
	{
		const float theta = glm::radians(-90.0f + 180.0f * i / slices);
		vertices[vertexParams * (i + 1)] = radius * glm::cos(theta);
		vertices[vertexParams * (i + 1) + 1] = radius * glm::sin(theta);
		vertices[vertexParams * (i + 1) + 2] = 0.0f;
		vertices[vertexSize / 2 + vertexParams * (i + 1)] = radius * glm::cos(theta);
		vertices[vertexSize / 2 + vertexParams * (i + 1) + 1] = radius * glm::sin(theta);
		vertices[vertexSize / 2 + vertexParams * (i + 1) + 2] = length;

		vertices[vertexParams * (i + 1) + 3] = textureRepeat * (0.5 + 0.5 * glm::cos(theta));
		vertices[vertexParams * (i + 1) + 4] = textureRepeat * (0.5 + 0.5 * glm::sin(theta));

		vertices[vertexSize / 2 + vertexParams * (i + 1) + 3] = textureRepeat * (1 + 0.5 + 0.5 * glm::cos(theta));
		vertices[vertexSize / 2 + vertexParams * (i + 1) + 4] = textureRepeat * (1 + 0.5 + 0.5 * glm::sin(theta));
	}

	indices = new GLuint[12 * slices + 6];
	for (auto i = 0; i < slices; ++i)
	{
		indices[12 * i] = 0;
		indices[12 * i + 1] = i + 1;
		indices[12 * i + 2] = (i + 2);
		indices[12 * i + 3] = slices + 2;
		indices[12 * i + 4] = slices + 2 + i + 1;
		indices[12 * i + 5] = (slices + 2 + i + 2);
		indices[12 * i + 6] = i + 1;
		indices[12 * i + 7] = (i + 2);
		indices[12 * i + 8] = slices + 2 + i + 1;
		indices[12 * i + 9] = (i + 2);
		indices[12 * i + 10] = slices + 2 + i + 1;
		indices[12 * i + 11] = (slices + 2 + i + 2);
	}
	indices[12 * slices] = slices + 1;
	indices[12 * slices + 1] = 1;
	indices[12 * slices + 2] = 2 * (slices + 2) - 1;
	indices[12 * slices + 3] = 1;
	indices[12 * slices + 4] = 2 * (slices + 2) - 1;
	indices[12 * slices + 5] = slices + 2 + 1;

	calculateVertexNormals();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (12 * slices + 6) * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexParams * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexParams * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertexParams * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void HalfCylinder::draw() const
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 12 * slices + 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void HalfCylinder::calculateVertexNormals() const
{
	const int vertexNormalsCount = 2 * (slices + 1) + 2;
	glm::vec3 *vertexNormals = new glm::vec3[vertexNormalsCount];
	const int faceNormalsCount = 4 * slices + 2;
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
		glm::vec3 insidePoint = { radius / 100 , radius / 100, length / 2 };
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