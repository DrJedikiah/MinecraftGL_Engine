#version 420 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bordersTexture;
uniform sampler2D ambientOcclusion;

vec2 texelSize = 1.f / textureSize(ambientOcclusion, 0);
const int size = 3;

float kernel[size*size] = 
{
    1.0 , 2.0 , 1.0 , 
    2.0 , 4.0 , 2.0 ,
    1.0 , 2.0 , 1.0 , 
};

void main()
{

	float occlusion = 0;
	for( int x = 0; x < size; ++x )
		for( int y = 0; y < size; ++y )
		{
			vec2 pos = vec2(TexCoords.x + (x - size/2)*texelSize.x, TexCoords.y + (y- size/2)*texelSize.y);
			occlusion += kernel[x + y * size] * texture(ambientOcclusion, pos).x ;
		}
		occlusion /= 16;

	float borders = 0;
	for( int x = 0; x < size; ++x )
		for( int y = 0; y < size; ++y )
		{
			vec2 pos = vec2(TexCoords.x + (x - size/2)*texelSize.x, TexCoords.y + (y- size/2)*texelSize.y);
			borders += kernel[x + y * size] * texture(bordersTexture, pos).x ;
		}
		borders /= 16;

	vec3 albedo = texture(screenTexture, TexCoords).xyz;

	FragColor = vec4( occlusion*albedo *borders , 1);
	//FragColor = vec4( albedo, 1);
}  