#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

//Camera matrix
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//Light matrix
uniform mat4 viewLight;
uniform mat4 projectionLight;
uniform mat4 projectionLightLarge;

out vec2 texCoord;
out vec3 fragPos;
out vec3 normal;
out vec4 fragPosLightSpace;
out vec4 fragPosLightSpaceLarge;

void main()
{
	texCoord = aTexCoord;
    fragPos = vec3(model * vec4(aPos, 1.0));
    normal = mat3(model) * aNormal;

	fragPosLightSpace = projectionLight * viewLight * model * vec4(aPos, 1.0);
	fragPosLightSpaceLarge = projectionLightLarge * viewLight * model * vec4(aPos, 1.0);

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}