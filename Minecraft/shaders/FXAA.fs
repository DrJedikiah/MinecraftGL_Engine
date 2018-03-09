


/*#version 330 core

// In
in vec2 TexCoords;

// Out
out vec3 color;

// Uniform
uniform sampler2D colorTexture;
uniform float     FXAASpan;
uniform float     FXAAReduceMul;
uniform float     FXAAReduceMin;

void main( void )
{
    vec2 frameBufSize = vec2(1600, 900);
    vec3 rgbNW = texture(colorTexture,TexCoords + (vec2(-1.0,-1.0) / frameBufSize)).xyz;
    vec3 rgbNE = texture(colorTexture,TexCoords + (vec2( 1.0,-1.0) / frameBufSize)).xyz;
    vec3 rgbSW = texture(colorTexture,TexCoords + (vec2(-1.0, 1.0) / frameBufSize)).xyz;
    vec3 rgbSE = texture(colorTexture,TexCoords + (vec2( 1.0, 1.0) / frameBufSize)).xyz;
    vec3 rgbM  = texture(colorTexture,TexCoords).xyz;

    vec3 luma    = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max(
        (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAAReduceMul),
        FXAAReduceMin);

    float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);

    dir = min(vec2( FXAASpan,  FXAASpan),
          max(vec2(-FXAASpan, -FXAASpan),
          dir * rcpDirMin)) / frameBufSize;

    vec3 rgbA = (1.0/2.0) * (
        texture(colorTexture, TexCoords + dir * (1.0/3.0 - 0.5)).xyz +
        texture(colorTexture, TexCoords + dir * (2.0/3.0 - 0.5)).xyz);

    vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
        texture(colorTexture, TexCoords + dir * (0.0/3.0 - 0.5)).xyz +
        texture(colorTexture, TexCoords + dir * (3.0/3.0 - 0.5)).xyz);

    float lumaB = dot(rgbB, luma);

    if((lumaB < lumaMin) || (lumaB > lumaMax))
    {
        color = rgbA;
    }
    else
    {
        color = rgbB;
    }

	color = texture(colorTexture,TexCoords).xyz;
}*/


#version 420 core

out vec4 FragColor;

in vec2 TexCoords;

// Uniform
uniform sampler2D screenTexture;
uniform float     FXAASpan;
uniform float     FXAAReduceMul;
uniform float     FXAAReduceMin;

uniform bool activated;

void main()
{
	if( !activated)
	{
		vec3 albedo = texture(screenTexture, TexCoords).xyz;
		FragColor = vec4( albedo , 1);
		return;
	}

	vec2 frameBufSize = vec2(1600, 900);
    vec3 rgbNW = texture(screenTexture,TexCoords + (vec2(-1.0,-1.0) / frameBufSize)).xyz;
    vec3 rgbNE = texture(screenTexture,TexCoords + (vec2( 1.0,-1.0) / frameBufSize)).xyz;
    vec3 rgbSW = texture(screenTexture,TexCoords + (vec2(-1.0, 1.0) / frameBufSize)).xyz;
    vec3 rgbSE = texture(screenTexture,TexCoords + (vec2( 1.0, 1.0) / frameBufSize)).xyz;
    vec3 rgbM  = texture(screenTexture,TexCoords).xyz;

    vec3 luma    = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max(
        (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAAReduceMul),
        FXAAReduceMin);

    float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);

    dir = min(vec2( FXAASpan,  FXAASpan),
          max(vec2(-FXAASpan, -FXAASpan),
          dir * rcpDirMin)) / frameBufSize;

    vec3 rgbA = (1.0/2.0) * (
        texture(screenTexture, TexCoords + dir * (1.0/3.0 - 0.5)).xyz +
        texture(screenTexture, TexCoords + dir * (2.0/3.0 - 0.5)).xyz);

    vec3 rgbB = rgbA * (1.0/2.0) + (1.0/4.0) * (
        texture(screenTexture, TexCoords + dir * (0.0/3.0 - 0.5)).xyz +
        texture(screenTexture, TexCoords + dir * (3.0/3.0 - 0.5)).xyz);

    float lumaB = dot(rgbB, luma);

    if((lumaB < lumaMin) || (lumaB > lumaMax))
    {
        FragColor = vec4(rgbA, 1);
    }
    else
    {
        FragColor = vec4(rgbB, 1);
    }

	//vec3 albedo = texture(screenTexture, TexCoords).xyz;
	//FragColor = vec4( 0.1*albedo , 1);
}  