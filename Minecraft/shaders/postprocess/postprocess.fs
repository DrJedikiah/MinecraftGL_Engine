#version 420 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bordersTexture;
uniform sampler2D ambientOcclusion;
uniform sampler2D depth;

vec2 texelSize = 1.f / textureSize(ambientOcclusion, 0);


const int size = 5;
float kernel[size*size] = 
{
    0.5 , 1.0 , 2.0 , 1.0 , 0.5, 
    1.0 , 2.0 , 2.0 , 2.0 , 1.0 , 
    2.0 , 2.0 , 2.0 , 2.0 , 2.0 , 
	1.0 , 2.0 , 2.0 , 2.0 , 1.0 ,
    0.5 , 1.0 , 2.0 , 1.0 , 0.5 
};


void main()
{
	float occlusion = 1;
	for( int x = 0; x < size; ++x )
		for( int y = 0; y < size; ++y )
		{
			vec2 pos = TexCoords +  vec2(x - size/2,y- size/2) * texelSize ;
			occlusion += kernel[x + y * size] * texture(ambientOcclusion, pos).x ;
		}
	occlusion /= 26+10;
	//occlusion = texture(ambientOcclusion, TexCoords).x ;

	float borders  = texture(bordersTexture, TexCoords).x;
	vec3 albedo = texture(screenTexture, TexCoords).xyz;

	FragColor = vec4( occlusion * borders * albedo, 1);
	//FragColor = vec4(  vec3( occlusion), 1);
}  