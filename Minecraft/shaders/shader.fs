#version 330 core

uniform sampler2D shadowMap;
uniform sampler2D textureBlocks;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 viewPos;

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;
in vec4 fragPosLightSpace;

out vec4 FragColor;

//Usefull vectors
vec3 lightDir = normalize(lightPos - fragPos);  
vec3 viewDir = normalize(viewPos - fragPos);

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z; 

	float bias = max(0.00003 * (1.0 - dot(normal, lightDir)), 0.00003);  

	float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  

    return 1-shadow;
}  

void main()
{
	vec4 texColor = texture(textureBlocks, texCoord);
	float diffuse = max(dot(normal, lightDir), 0.0);
	float spec = pow(max(dot( normalize(viewDir+lightDir), normal), 0.0), 256);
	float shadow = ShadowCalculation(fragPosLightSpace);     
	FragColor =  vec4( (0.1 + shadow * (diffuse+spec)) * texColor.xyz  * lightColor , texColor.w  );
}