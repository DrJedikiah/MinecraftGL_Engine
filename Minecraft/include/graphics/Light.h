#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/map/World.h"

#include "graphics/Shader.h"
#include "graphics/FrameBuffer.h"
#include "graphics/Shader.h"

class Light
{
public:
	Light(glm::vec3 colorLight = glm::vec3(1, 1, 1));
	virtual ~Light();

	glm::vec3 color;
};


class DirectionalLight : public Light
{
public:
	DirectionalLight(glm::vec3 directionLight, float size = 20.f);
	virtual ~DirectionalLight();
	void BakeShadows(glm::vec3 lookPoint);

	glm::mat4 ProjectionView();

	ShadowMapFBO fbo;

private:
	glm::vec3 direction;
	const glm::mat4 projection;
	glm::mat4 view;

	static Shader * shader;

};

class PointLight : public Light
{
public:
	PointLight(glm::vec3 positionLight, float radiusLight);



	glm::vec3 position;
	float radius;


};

class AmbientLight : public Light
{
public:
	AmbientLight( float intensityLight);



	float intensity;

};