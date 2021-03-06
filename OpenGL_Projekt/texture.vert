#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 normals;

out vec2 UV;
out vec3 Normal;
out vec3 fragmentPosition;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * transform * vec4(position, 1.0f);
	UV = textureCoord;
	fragmentPosition = vec3(transform * vec4(position, 1.0));
	Normal = mat3(transpose(inverse(transform))) * normals;
}