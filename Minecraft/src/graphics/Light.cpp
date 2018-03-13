#include "graphics/Light.h"

/////////////////////////// Light ///////////////////////////

Light::Light(glm::vec3 colorLight) : 
	color(colorLight)
{
}

Light::~Light(){}

/////////////////////////// DirectionalLight ///////////////////////////

Shader* DirectionalLight::shader = nullptr;

DirectionalLight::DirectionalLight(glm::vec3 directionLight, float size) :
	fbo(2 * 2048, 2 * 2048),
	direction(directionLight),
	projection(glm::ortho(-size, size, -size, size, 0.1f, 1000.f))
{
	if (!shader)
		shader = new Shader("shaders/forward/shadows.vs", "shaders/forward/shadows.fs");
}

DirectionalLight::~DirectionalLight()
{
	if (shader)
	{
		delete(shader);
		shader = nullptr;
	}
}


void DirectionalLight::BakeShadows(glm::vec3 lookPoint)
{
	view = glm::lookAt(
		lookPoint - 500.f*direction,
		lookPoint,
		glm::vec3(0.0f, 1.0f, 0.0f));

	shader->Use();

	fbo.Use();
	fbo.Clear();
	shader->setMat4("projview", projection * view);
	World::DrawOpaque(*shader);
}

glm::mat4 DirectionalLight::ProjectionView() { return projection * view; }

/////////////////////////// PointLight ///////////////////////////

PointLight::PointLight(glm::vec3 positionLight, float radiusLight ) :
	position(positionLight),
	radius(radiusLight)
{
}

/////////////////////////// AmbientLight ///////////////////////////

AmbientLight::AmbientLight(float intensityLight) :
	intensity(intensityLight)
{
}
