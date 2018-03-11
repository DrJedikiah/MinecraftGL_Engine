#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;

uniform sampler2D textureBlocks;

void main()
{             
	FragColor = texture(textureBlocks, texCoord);
}