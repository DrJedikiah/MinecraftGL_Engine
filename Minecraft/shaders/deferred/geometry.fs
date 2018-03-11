#version 420 core

layout(location = 0) out vec4 gColor;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gPosition;

uniform sampler2D textureBlocks;

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;

void main()
{
	gColor = texture(textureBlocks, texCoord);

	if( gColor.a == 0)
		discard;

	gNormal = vec4(normalize(normal),1);
	gPosition = vec4(fragPos,1);
}

	