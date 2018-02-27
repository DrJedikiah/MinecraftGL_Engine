#version 330 core

in vec2 texCoord;
in vec3 FragPos;
in vec3 Normal;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define MAX_LIGHTS 100
uniform Light lights[MAX_LIGHTS];
uniform int numLights;

uniform sampler2D tex;
uniform float ambient;
uniform vec3 viewPos;
uniform float near; 
uniform float far; 

out vec4 FragColor;
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	//Data
    vec4 texColor = texture(tex, texCoord);
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	float diffuse = 0.f;
	float spec = 0.f;

	for(int i=0;i<numLights;++i)
	{
		vec3 lightDir = normalize(lights[i].position - FragPos);  

		//Diffuse
		diffuse += max(dot(norm, lightDir), 0.0);

		//Specular
		spec += pow(max(dot( normalize(viewDir+lightDir), norm), 0.0), 256);
	}
	vec3 lightColor = vec3(228.f/256.f,134/256.f,0);

	vec3 result = (ambient + diffuse + spec) * texColor.xyz * lightColor;

	FragColor = vec4(result, texColor.w);

    //float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
	// FragColor = vec4(vec3(depth), 1.0);

}