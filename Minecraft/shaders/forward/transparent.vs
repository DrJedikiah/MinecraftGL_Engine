#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 projView;

out vec2 texCoord;
out vec3 fragPos;
out vec3 normal;

void main()
{
	texCoord = aTexCoord;
    fragPos = vec3(model * vec4(aPos, 1.0));
    normal = mat3(model) * aNormal;

    gl_Position = projView * model * vec4(aPos, 1.0);
} 