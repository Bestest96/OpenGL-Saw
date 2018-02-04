#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D Tex;

void main()
{
	vec3 textureColor = texture(Tex, UV).rgb;
	color = vec4(textureColor, 1.0);
}