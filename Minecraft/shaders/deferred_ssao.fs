#version 420 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D gColor;
uniform sampler2D gNormal;
uniform sampler2D gPosition;
uniform sampler2D gDepth;

uniform sampler2D texNoise;

uniform float near; 
uniform float far; 
uniform vec3 samples[64];

uniform mat4 view;
uniform mat4 projection;

vec2 texelSize = 1.f / textureSize(gPosition, 0);
const vec2 noiseScale = vec2(1500/4.0, 900/4.0);

  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}



void main()
{
	float depth = texture(gDepth, TexCoords).x;
	if( depth == 1)
	{
		FragColor = vec4(vec3(1),1);
		return;
	}


	vec3 fragPos   = texture(gPosition, TexCoords).xyz;
	vec3 normal    = texture(gNormal, TexCoords).rgb;
	vec3 randomVec = texture(texNoise, TexCoords * noiseScale).xyz;  

	vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN       = mat3(tangent, bitangent, normal); 

	int kernelSize = 64;
	float radius = 1;
	float bias = 0.001;

	float occlusion = 0.0;
	for(int i = 0; i < kernelSize; ++i)
	{
		// get sampl position
		vec3 sampl = TBN * samples[i]; // From tangent to view-space
		sampl = fragPos + sampl * radius; 
    
		vec4 offset = vec4(sampl, 1.0);
		offset      = projection * view * offset;    // from view to clip-space
		offset.xyz /= offset.w;               // perspective divide
		offset.xyz  = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0  

		float sampleDepth = texture(gDepth, offset.xy).x;
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(depth - sampleDepth));

		if( sampleDepth + bias < depth)
			occlusion += 1*rangeCheck;//(sampleDepth >= depth + bias ? 1.0 : 0.0) * rangeCheck;    
	}  
	occlusion = occlusion / kernelSize;
	FragColor = vec4(vec3(1 - occlusion*occlusion),1);
}  