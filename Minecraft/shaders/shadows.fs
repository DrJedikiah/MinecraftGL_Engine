#version 330 core

out vec4 FragColor;

float near = 1.f;
float far = 10f;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{     
	float depth = LinearizeDepth(gl_FragCoord.z)/far;
	FragColor = vec4(depth,depth,depth, 1);
}