#pragma once

#include "engine/Entity.h"
#include "graphics/Model.h"
#include "engine/Cube.h"

class PlayerAvatar : public Entity
{
public: 
	PlayerAvatar( float mass = 1.f, float height = 0.9f, float radius = 0.4f);
	
	void Update(float delta) override;
	void Draw(const Shader& shader) const override;
	void UpdateModels() override;

	const float height;
	const float radius;

private:
	Model m_modelBody;
};