#version 420 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D depthTexture;
uniform sampler2D transTexture;

vec2 texelSize = 1.f / textureSize(depthTexture, 0);

const int size = 3;
float kernel[size*size] = 
{
1,1,1,
1,-8,1,
1,1,1
};

vec3 blurredColor();

void main()
{
	float average = 0;
	float averageTrans = 0;

	for( int x = 0; x < size; ++x )
		for( int y = 0; y < size; ++y )
		{
			vec2 pos = vec2(TexCoords.x + (x - size/2)*texelSize.x, TexCoords.y + (y- size/2)*texelSize.y);
			
			float depth = texture(depthTexture,pos).x;
			average +=  50*kernel[x + y * size] * depth;

			vec3 transCol =  texture(transTexture,pos).rgb;
			float gray = (transCol.r+transCol.g+transCol.b)/3.f;
			averageTrans += 10*kernel[x + y * size] * gray;
		}

	averageTrans = max(1 - averageTrans,0);

	vec4 colorTrans = vec4( 0.5f  *(vec3(averageTrans) + blurredColor()) + 0.5f  *vec3(1,1,1), 1);
	vec4 colorOpaque = vec4(vec3(1 - average*average),1);

	FragColor = colorTrans * colorOpaque;
}  

float kernelBlur[9] = 
{
1,2,1,
2,4,2,
1,2,1
};

vec3 blurredColor()
{
	vec3 color = vec3(0,0,0);
	for( int x = 0; x < 3; ++x )
		for( int y = 0; y < 3; ++y )
		{
			vec2 pos = TexCoords +  vec2(x - 1,y - 1) * texelSize ;
			color += kernelBlur[x + y * 3] * texture(transTexture, pos).xyz ;
		}
	color /= 16;
	return color;
}