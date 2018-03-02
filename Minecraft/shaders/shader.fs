#version 330 core

uniform sampler2D shadowMap;
uniform sampler2D shadowMapLarge;
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

//Usefull vectors
vec3 lightDir = normalize(lightPos - fragPos);  
vec3 viewDir = normalize(viewPos - fragPos);

float ShadowCalculation(vec4 frag, sampler2D map, float threshold)
{
    frag = frag * 0.5 + 0.5;
	vec2 texelSize = 1.0 / textureSize(map, 0);
	float bias = max(threshold * (1.0 - dot(normal, lightDir)), threshold);  
    float currentDepth = frag.z; 

	float shadow = 0.f;
	for( int x = -1; x <= 1; ++x)
		for( int y = -1; y <= 1; ++y)
		{
			float depth = texture(map, vec2(frag.x + x * texelSize.x, frag.y + y + texelSize.y)).r;
			shadow += currentDepth - bias > depth ? 1.0 : 0.0;  
		}
	shadow /= 3*3;
    return 1 -shadow;
}  

void main()
{

	vec4 texColor = texture(textureBlocks, texCoord);
	float diffuse = max(dot(normal, lightDir), 0.0);
	float spec = pow(max(dot( normalize(viewDir+lightDir), normal), 0.0), 256);

	float shadow = 1;
	if( fragPosLightSpace.x <= 1.f && fragPosLightSpace.x >= -1.f && fragPosLightSpace.y <= 1.f && fragPosLightSpace.y >= -1.f)
		shadow = ShadowCalculation(fragPosLightSpace, shadowMap, 0.00002f);  
	else if (fragPosLightSpaceLarge.x <= 1.f && fragPosLightSpaceLarge.x >= -1.f && fragPosLightSpaceLarge.y <= 1.f && fragPosLightSpaceLarge.y >= -1.f)
		shadow = ShadowCalculation(fragPosLightSpaceLarge, shadowMapLarge, 0.00013f);  

	FragColor =  vec4( (0.3 + shadow * (diffuse+spec)) * texColor.xyz  * lightColor , texColor.w  );
}

	