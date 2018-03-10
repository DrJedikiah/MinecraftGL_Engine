#version 420 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D gNormal;
uniform sampler2D gPosition;
uniform sampler2D gDepth;
uniform sampler2D texNoise;
uniform vec3 samples[64];
uniform mat4 projView;

const vec2 noiseScale = vec2(1500/4.0, 900/4.0);

void main()
{
	float depth = texture(gDepth, TexCoords).x;
	float weight = (depth >= 0.98 ? 1 - (depth - 0.98) / 0.009:1);

	vec3 fragPos   = texture(gPosition, TexCoords).xyz;
	vec3 normal    = texture(gNormal, TexCoords).rgb;
	vec3 randomVec = texture(texNoise, TexCoords * noiseScale).xyz;  

	vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN       = mat3(tangent, bitangent, normal); 

	int kernelSize = 64;
	float radius = 1;
	float bias = 0.00005;

	float occlusion = 0.0;
	for(int i = 0; i < kernelSize; ++i)
	{
		vec3 sampl = TBN * samples[i];
		sampl = fragPos + sampl * radius; 
    
		vec4 offset = projView * vec4(sampl, 1.0);
		offset.xyz /= offset.w; 
		offset.xyz  = offset.xyz * 0.5 + 0.5;

		float sampleDepth = texture(gDepth, offset.xy).x;
		if( sampleDepth + bias < depth)
			if( offset.x <= 1.f && offset.x >= 0 && offset.y <= 1.f && offset.y >= 0)
				occlusion += 1.1;
	}  
	occlusion = occlusion / kernelSize;
	FragColor = vec4(vec3(1 - weight*occlusion),1);
} 