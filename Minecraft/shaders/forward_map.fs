#version 330 core

uniform sampler2D textureBlocks;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 viewPos;

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;
in vec4 fragPosLightSpace;
in vec4 fragPosLightSpaceLarge;

out vec4 FragColor;

void main()
{

	vec4 texColor = texture(textureBlocks, texCoord);
	FragColor = texColor;
}

	