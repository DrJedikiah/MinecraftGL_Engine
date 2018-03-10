#version 330 core

out vec4 FragColor;

float near = 0.02;
float far = 1000;
float LinearizeDepth(float depth) 
{
	return depth;
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{             
	FragColor = vec4(1,0,0,1);
}