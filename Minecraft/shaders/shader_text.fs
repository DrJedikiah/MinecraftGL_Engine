#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D tex;

void main()
{
	//
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, texCoord).r);
    FragColor = sampled;
	
} 