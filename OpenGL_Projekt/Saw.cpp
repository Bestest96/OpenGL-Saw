#include "Saw.h"

Saw::~Saw()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	delete[] vertices;
	delete[] indices;
}

Saw::Saw(): Saw(10, 1.0, 2.0, 10.0)
{
}

Saw::Saw(GLuint pSpikeCount, GLfloat pWidth, GLfloat pHeight, GLfloat pLength, GLuint pTextureRepeat): spikeCount(pSpikeCount), width(pWidth), height(pHeight), length(pLength), vertexParams(8), textureRepeat(pTextureRepeat)
{
	const int vertexSize = vertexParams * (4 + spikeCount + 2 * (spikeCount + 1));
	vertices = new GLfloat[vertexParams * (4 + spikeCount + 2 * (spikeCount + 1))];
	vertices[0] = -width / 2;
	vertices[1] = 0.0;
	vertices[2] = 0.0;
	vertices[3] = textureRepeat * 1.0;
	vertices[4] = textureRepeat * 1.0;
	vertices[8] = width / 2;
	vertices[9] = 0.0;
	vertices[10] = 0.0;
	vertices[11] = textureRepeat * 0.0;
	vertices[12] = textureRepeat * 1.0;
	vertices[16] = -width / 2;
	vertices[17] = 0.0;
	vertices[18] = length;
	vertices[19] = textureRepeat * 0.0;
	vertices[20] = textureRepeat * 1.0;
	vertices[24] = width / 2;
	vertices[25] = 0.0;
	vertices[26] = length;
	vertices[27] = textureRepeat * 1.0;
	vertices[28] = textureRepeat * 1.0;
	vertices[32] = -width / 2;
	vertices[33] = -height / 2;
	vertices[34] = 0.0;
	vertices[35] = textureRepeat * 1.0;
	vertices[36] = textureRepeat * 0.0;
	vertices[40] = width / 2;
	vertices[41] = -height / 2;
	vertices[42] = 0.0;
	vertices[43] = textureRepeat * 0.0;
	vertices[44] = textureRepeat * 0.0;
	vertices[48] = -width / 2;
	vertices[49] = -height / 2;
	vertices[50] = length;
	vertices[51] = textureRepeat * 0.0;
	vertices[52] = textureRepeat * 0.0;
	vertices[56] = width / 2;
	vertices[57] = -height / 2;
	vertices[58] = length;
	vertices[59] = textureRepeat * 1.0;
	vertices[60] = textureRepeat * 0.0;
	for (unsigned int i = 0; i < spikeCount - 1; ++i)
	{
		GLfloat ratio = GLfloat(i + 1) / GLfloat(spikeCount);
		vertices[64 + 2 * vertexParams * i] = -width / 2;
		vertices[64 + 2 * vertexParams * i + 1] = -height / 2;
		vertices[64 + 2 * vertexParams * i + 2] = ratio * length;
		vertices[64 + 2 * vertexParams * i + 3] = textureRepeat * (1.0f - ratio);
		vertices[64 + 2 * vertexParams * i + 4] = textureRepeat * 0.0;
		vertices[64 + 2 * vertexParams * i + 8] = width / 2;
		vertices[64 + 2 * vertexParams * i + 9] = -height / 2;
		vertices[64 + 2 * vertexParams * i + 10] = ratio * length;
		vertices[64 + 2 * vertexParams * i + 11] = textureRepeat * ratio;
		vertices[64 + 2 * vertexParams * i + 12] = textureRepeat * 0.0;
	}
	for (unsigned int i = 0; i < spikeCount; ++i)
	{
		GLfloat ratio = GLfloat(i) / GLfloat(spikeCount);
		vertices[64 + vertexParams * 2 * (spikeCount - 1) + vertexParams * i] = 0.0;
		vertices[64 + vertexParams * 2 * (spikeCount - 1) + vertexParams * i + 1] = -height;
		vertices[64 + vertexParams * 2 * (spikeCount - 1) + vertexParams * i + 2] = length / (2 * spikeCount) + ratio * length;
		vertices[64 + vertexParams * 2 * (spikeCount - 1) + vertexParams * i + 3] = textureRepeat * 1.0;
		vertices[64 + vertexParams * 2 * (spikeCount - 1) + vertexParams * i + 4] = textureRepeat * 1.0;
	}
	indices = new GLuint[3 * (10 + 4 * spikeCount)];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;
	indices[6] = 0;
	indices[7] = 4;
	indices[8] = 5;
	indices[9] = 0;
	indices[10] = 1;
	indices[11] = 5;
	indices[12] = 1;
	indices[13] = 3;
	indices[14] = 7;
	indices[15] = 1;
	indices[16] = 5;
	indices[17] = 7;
	indices[18] = 0;
	indices[19] = 4;
	indices[20] = 6;
	indices[21] = 0;
	indices[22] = 2;
	indices[23] = 6;
	indices[24] = 3;
	indices[25] = 7;
	indices[26] = 2;
	indices[27] = 2;
	indices[28] = 7;
	indices[29] = 6;

	indices[30] = 4;
	indices[31] = 5;
	indices[32] = 8 + 2 * (spikeCount - 1);
	indices[33] = 4;
	indices[34] = 8;
	indices[35] = 8 + 2 * (spikeCount - 1);
	indices[36] = 8;
	indices[37] = 9;
	indices[38] = 8 + 2 * (spikeCount - 1);
	indices[39] = 5;
	indices[40] = 9;
	indices[41] = 8 + 2 * (spikeCount - 1);
	for (unsigned int i = 1; i < spikeCount - 1; ++i)
	{
		indices[42 + 12 * (i - 1)] = 8 + 2 * (i - 1);
		indices[42 + 12 * (i - 1) + 1] = 8 + 2 * (i - 1) + 1;
		indices[42 + 12 * (i - 1) + 2] = 8 + 2 * (spikeCount - 1) + i;
		indices[42 + 12 * (i - 1) + 3] = 8 + 2 * i;
		indices[42 + 12 * (i - 1) + 4] = 8 + 2 * i + 1;
		indices[42 + 12 * (i - 1) + 5] = 8 + 2 * (spikeCount - 1) + i;
		indices[42 + 12 * (i - 1) + 6] = 8 + 2 * (i - 1) + 1;
		indices[42 + 12 * (i - 1) + 7] = 8 + 2 * i + 1;
		indices[42 + 12 * (i - 1) + 8] = 8 + 2 * (spikeCount - 1) + i;
		indices[42 + 12 * (i - 1) + 9] = 8 + 2 * (i - 1);
		indices[42 + 12 * (i - 1) + 10] = 8 + 2 * i;
		indices[42 + 12 * (i - 1) + 11] = 8 + 2 * (spikeCount - 1) + i;
	}
	indices[3 * (10 + 4 * spikeCount) - 12] = 8 + 2 * (spikeCount - 2);
	indices[3 * (10 + 4 * spikeCount) - 11] = 8 + 2 * (spikeCount - 2) + 1;
	indices[3 * (10 + 4 * spikeCount) - 10] = 8 + 2 * (spikeCount - 1) + spikeCount - 1;
	indices[3 * (10 + 4 * spikeCount) - 9] = 7;
	indices[3 * (10 + 4 * spikeCount) - 8] = 8 + 2 * (spikeCount - 2) + 1;
	indices[3 * (10 + 4 * spikeCount) - 7] = 8 + 2 * (spikeCount - 1) + spikeCount - 1;
	indices[3 * (10 + 4 * spikeCount) - 6] = 6;
	indices[3 * (10 + 4 * spikeCount) - 5] = 8 + 2 * (spikeCount - 2);
	indices[3 * (10 + 4 * spikeCount) - 4] = 8 + 2 * (spikeCount - 1) + spikeCount - 1;
	indices[3 * (10 + 4 * spikeCount) - 3] = 6;
	indices[3 * (10 + 4 * spikeCount) - 2] = 7;
	indices[3 * (10 + 4 * spikeCount) - 1] = 8 + 2 * (spikeCount - 1) + spikeCount - 1;

	calculateVertexNormals();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * (10 + 4 * spikeCount) * sizeof(GLuint), indices, GL_STATIC_DRAW);

	// vertex geometry data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexParams * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// vertex texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexParams * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertexParams * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
}

void Saw::calculateVertexNormals() const
{
	const int vertexNormalsCount = 4 + spikeCount + 2 * (spikeCount + 1);
	glm::vec3 *vertexNormals = new glm::vec3[vertexNormalsCount];
	const int faceNormalsCount = 10 + 4 * spikeCount;
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
		glm::vec3 insidePoint = { 0.0, - height / 4, length / 2 };
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

void Saw::draw() const
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 3 * (10 + 4 * spikeCount), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
