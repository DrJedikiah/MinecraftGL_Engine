#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 

#include <iostream>

#include "graphics/Texture.h"

//////////////////////////////// FBO ////////////////////////////////
class FBO  
{
public:
	FBO(int width, int height);

	static void UseDefault();
	static void ClearDefault();

	static void BlitDepth(FBO& origin, FBO& destination);

	virtual void Use() const = 0;
	virtual void Clear() const = 0;

//protected:
	GLuint m_fbo;
	int m_width;
	int m_height;
	int m_aaSamples;
	
};

//////////////////////////////// TextureDepthFBO ////////////////////////////////
class TextureDepthFBO : public FBO
{
public:
	TextureDepthFBO(int width, int height);
	~TextureDepthFBO();

	void Use() const override;
	void Clear() const override;
	void UseTexture(TextureUnit textureUnit)const;
	
private:
	unsigned int depth;
	unsigned int texture;
};

//////////////////////////////// GrayFBO ////////////////////////////////
class GrayFBO : public FBO
{
public:
	GrayFBO(int width, int height);
	~GrayFBO();

	void Use() const override;
	void Clear() const override;
	void UseTexture(TextureUnit textureUnit)const;

private:
	unsigned int texture;
};
//////////////////////////////// PostProcessingFBO ////////////////////////////////
class PostProcessingFBO : public FBO
{
public:
	PostProcessingFBO(int width, int height);
	~PostProcessingFBO();

	void Use() const override;
	void Clear() const override;
	void UseTexture(TextureUnit textureUnit)const;
	void UseDepth(TextureUnit textureUnit)const;
//private:
	unsigned int m_texture;
	unsigned int m_depth;
	unsigned int m_rbo;

};

//////////////////////////////// ShadowMapFBO ////////////////////////////////
class ShadowMapFBO : public FBO
{
public:
	ShadowMapFBO(int width, int height);
	~ShadowMapFBO();

	void Use() const override;
	void Clear() const override;
	void UseTexture(TextureUnit textureUnit)const;

private:
	unsigned int depthMap;
};

//////////////////////////////// DeferredFBO ////////////////////////////////
class DeferredFBO : public FBO
{
public:
	DeferredFBO(int width, int height);
	~DeferredFBO();

	void Use() const override;
	void Clear() const override;
	void UseColor(TextureUnit textureUnit) const;
	void UseNormal(TextureUnit textureUnit) const;
	void UsePosition(TextureUnit textureUnit) const;
	void UseDepth(TextureUnit textureUnit) const;
	
//private:
	GLuint gDepth;
	GLuint gColor;
	GLuint gNormal;
	GLuint gPosition;
	

};