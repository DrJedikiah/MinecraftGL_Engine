#version 420 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D gColor;
uniform sampler2D gNormal;
uniform sampler2D gPosition;

uniform sampler2D shadowMap;
uniform sampler2D shadowMapLarge;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform mat4 viewLight;
uniform mat4 projectionLight;
uniform mat4 projectionLightLarge;

vec4 color = texture(gColor, TexCoords);
vec3 fragPos = texture(gPosition, TexCoords).xyz;
vec3 normal = texture(gNormal, TexCoords).xyz;

vec3 lightDir = normalize(lightPos - fragPos);  
vec3 viewDir = normalize(viewPos - fragPos);

float ShadowCalculation(vec3 frag, sampler2D map, float threshold);

void main()
{
	float ambient =  0.3f;
	float diffuse = max(dot(normal, lightDir), 0.0);
	float spec = pow(max(dot( normalize(viewDir+lightDir), normal), 0.0), 16);

	float shadow = 1;

	vec4 fragPosLightSpace = projectionLight * viewLight * vec4(fragPos,1);
	vec4 fragPosLightSpaceLarge = projectionLightLarge * viewLight * vec4(fragPos,1);

	if( fragPosLightSpace.x <= 1.f && fragPosLightSpace.x >= -1.f && fragPosLightSpace.y <= 1.f && fragPosLightSpace.y >= -1.f)
		shadow = ShadowCalculation(fragPosLightSpace.xyz, shadowMap, 0.00015f);  
	/*else if (fragPosLightSpaceLarge.x <= 1.f && fragPosLightSpaceLarge.x >= -1.f && fragPosLightSpaceLarge.y <= 1.f && fragPosLightSpaceLarge.y >= -1.f)
		shadow = ShadowCalculation(fragPosLightSpaceLarge.xyz, shadowMapLarge, 0.0002f); */

	FragColor =  vec4( (ambient + shadow * (diffuse+spec)) * color.xyz  * lightColor , color.w  );
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
    return 1 -shadow;
} 