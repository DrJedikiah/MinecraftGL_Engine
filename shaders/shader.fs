#version 330 core

in vec2 texCoord;
in vec3 FragPos;
in vec3 Normal;

#define MAX_LIGHTS 100
uniform vec3 lights[MAX_LIGHTS];
uniform int numLights;

uniform sampler2D tex;
uniform float ambient;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
	//Data
    vec4 color = texture(tex, texCoord);
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	float diffuse = 0.f;
	float spec = 0.f;

	for(int i=0;i<numLights;++i)
	{
		vec3 lightDir = normalize(lights[i] - FragPos);  

		//Diffuse
		diffuse += max(dot(norm, lightDir), 0.0);

		//Specular
		spec += pow(max(dot( normalize(viewDir+lightDir), norm), 0.0), 256);
	}

	vec3 result = (ambient + diffuse + spec) * color.xyz;
	FragColor = vec4(result, 1.0);

}