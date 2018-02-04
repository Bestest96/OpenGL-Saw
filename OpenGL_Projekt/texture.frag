#version 330 core

in vec2 UV;
in vec3 Normal;
in vec3 fragmentPosition;

out vec4 color;

uniform sampler2D Tex;
uniform vec3 lightColor;
uniform vec3 lightPosition;

void main()
{
    vec3 norm = normalize(Normal);
	vec3 lightDirection = normalize(lightPosition - fragmentPosition);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * lightColor;
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
	vec3 textureColor = texture(Tex, UV).rgb;
	vec3 result = (ambient + diffuse) * textureColor;
	color = vec4(result, 1.0);
}