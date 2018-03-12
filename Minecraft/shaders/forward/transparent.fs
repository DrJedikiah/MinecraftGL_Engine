#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;

uniform sampler2D textureBlocks;
uniform sampler2D depthTexture;

void main()
{             
	vec2 screenPos = gl_FragCoord.xy/vec2(1280, 720);
	float worldDepth = texture(depthTexture, screenPos).x;
	float depth = gl_FragCoord.z;

	if( depth < worldDepth)
		FragColor = texture(textureBlocks, texCoord);
	else
		discard;
	
}