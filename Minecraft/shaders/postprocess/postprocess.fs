#version 420 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bordersTexture;


void main()
{

	float borders  = texture(bordersTexture, TexCoords).x;
	vec3 albedo = texture(screenTexture, TexCoords).xyz;

	FragColor = vec4( borders * albedo, 1);
	//FragColor = vec4(  vec3( borders), 1);
}  