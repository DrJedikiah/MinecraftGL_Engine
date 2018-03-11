#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
    TexCoords = aTexCoords;
} 

/*
#version 330 core

// In
layout(location = 0) in vec2 vertexCoord;

// Out
out vec2 textureUV;

void main(void)
{
  gl_Position = vec4(vertexCoord, 0.0, 1.0);
  textureUV   = (vertexCoord + 1.0) / 2.0;
}*/