#version 420 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D gColor;
uniform sampler2D gNormal;
uniform sampler2D gPosition;
uniform sampler2D shadowMap;
uniform sampler2D shadowMapLarge;
uniform sampler2D ambientOcclusion;

uniform vec3 viewPos;
uniform vec3 lightDir;
uniform vec3 lightColor;

uniform mat4 projectionViewLight;
uniform mat4 projectionViewLightLarge;

vec4 color = texture(gColor, TexCoords);
vec3 fragPos = texture(gPosition, TexCoords).xyz;
vec3 normal = texture(gNormal, TexCoords).xyz;

vec3 viewDir = normalize(viewPos - fragPos);

float ShadowCalculation(vec3 frag, sampler2D map, float threshold);
float blurredAmbientOcclusion();

void main()
{
	float ambient =  0.3f;
	float diffuse = max(dot(normal, lightDir), 0.0);
	float spec = pow(max(dot( normalize(viewDir+lightDir), normal), 0.0), 16);
	float occlusion = blurredAmbientOcclusion();

	float shadow = 1;

	vec4 fragPosLightSpace = projectionViewLight * vec4(fragPos,1);
	vec4 fragPosLightSpaceLarge = projectionViewLightLarge * vec4(fragPos,1);

	if( fragPosLightSpace.x < 1.f && fragPosLightSpace.x > -1.f && fragPosLightSpace.y < 1.f && fragPosLightSpace.y > -1.f)
		shadow = ShadowCalculation(fragPosLightSpace.xyz, shadowMap, 0.00015f);  
	else if (fragPosLightSpaceLarge.x <= 1.f && fragPosLightSpaceLarge.x >= -1.f && fragPosLightSpaceLarge.y <= 1.f && fragPosLightSpaceLarge.y >= -1.f)
		shadow = ShadowCalculation(fragPosLightSpaceLarge.xyz, shadowMapLarge, 0.0005f);  

	FragColor =  vec4( occlusion*( ambient +  (shadow*0.8 + 0.2) * diffuse + shadow * spec) * color.xyz  * lightColor , color.w  );

	//FragColor = vec4(vec3(texture(ambientOcclusion, TexCoords).x),1);
	//FragColor = vec4(vec3(occlusion),1);
}  




vec2 texelSize = 1.f / textureSize(ambientOcclusion, 0);
const int size = 5;
float kernel[size*size] = 
{
	1 , 2 , 3 , 2 , 1, 
    2 , 3 , 3 , 3 , 2 , 
    3 , 3 , 3 , 3 , 3 , 
	2 , 3 , 3 , 3 , 2 , 
    1 , 2 , 3 , 2 , 1, 
};

float blurredAmbientOcclusion()
{
	float occlusion = 1;
	for( int x = 0; x < size; ++x )
		for( int y = 0; y < size; ++y )
		{
			vec2 pos = TexCoords +  vec2(x - size/2,y- size/2) * texelSize ;
			occlusion += kernel[x + y * size] * texture(ambientOcclusion, pos).x ;
		}
	occlusion /=18+15+26;
	return occlusion;
}

float ShadowCalculation(vec3 frag, sampler2D map, float threshold)
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
    return 1 - shadow;
} 