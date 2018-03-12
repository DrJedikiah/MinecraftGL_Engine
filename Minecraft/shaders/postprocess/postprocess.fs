#version 420 core
out vec4 FragColor;



in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bordersTexture;
uniform sampler2D transtex;

void main()
{

	vec3 borders  = texture(bordersTexture, TexCoords).xyz;
	vec3 albedo = texture(screenTexture, TexCoords).xyz;
	vec4 transColor = texture(transtex, TexCoords);

	FragColor = vec4( borders * albedo + transColor.xyz, 1);
}  