#version 420 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D depthTexture;


vec2 texelSize = 1.f / textureSize(depthTexture, 0);

const int size = 3;
float kernel[size*size] = 
{
1,1,1,
1,-8,1,
1,1,1
};
  
void main()
{
	float average = 0;
	for( int x = 0; x < size; ++x )
		for( int y = 0; y < size; ++y )
		{
			vec2 pos = vec2(TexCoords.x + (x - size/2)*texelSize.x, TexCoords.y + (y- size/2)*texelSize.y);
			float depth = texture(depthTexture,pos).x;

			average +=  50*kernel[x + y * size] * depth;
		}

	FragColor = vec4(1-average*average,0,0,1);
}  