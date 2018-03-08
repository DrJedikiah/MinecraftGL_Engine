#version 420 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{


	vec3 albedo = texture(screenTexture, TexCoords).xyz;

	FragColor = vec4( albedo , 1);

}  