#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

#include "Texture.h"

class FrameBuffer
{
public:
	FrameBuffer(int width, int height);
	~FrameBuffer();

	void Use()const;
	void UseTexture(TextureUnit textureUnit)const;
	static void UseDefault();

	unsigned int fbo;
private:
	int m_width;
	int m_height;

	unsigned int rbo;
	unsigned int texture;
};

class FrameBufferShadowMap
{
public:
	FrameBufferShadowMap(int width, int height);
	~FrameBufferShadowMap();

	void Use()const;
	void UseTexture(TextureUnit textureUnit)const;

private:
	int m_width;
	int m_height;

	unsigned int depthMapFBO;
	unsigned int depthMap;
};

class GBuffer
{
public:
	GBuffer(int width, int height);
	~GBuffer();

	GLuint buffer;
	GLuint gColor;
	GLuint gNormal;
	GLuint gPosition;
	GLuint gDepth;

private:
	int m_width;
	int m_height;


};