#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

//Camera matrix
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;
out vec3 fragPos;
out vec3 normal;

void main()
{
	texCoord = aTexCoord;
    fragPos = vec3(model * vec4(aPos, 1.0));
    normal = mat3(model) * aNormal;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}