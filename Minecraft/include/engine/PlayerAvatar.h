#pragma once

#include "engine/Entity.h"
#include "graphics/Model.h"
#include "engine/Cube.h"

class PlayerAvatar : public Entity
{
public:
	PlayerAvatar();
	
	void Update(float delta) override;
	void Draw(const Shader& shader) const override;
	void UpdateModels() override;

private:
	Model m_modelBody;

};