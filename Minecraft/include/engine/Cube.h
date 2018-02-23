#pragma once

#include "engine/Entity.h"
#include "engine/Block.h"
#include "graphics/Model.h"
#include "graphics/TexturesBlocks.h"
#include "graphics/Drawable.h"
#include "util/Util.h"

class Cube : public Entity
{
public:
	Cube(float size = 1.f);

	void Draw(const Shader & shader) const override;
	void Update(float delta) override;
	void UpdateModels() override ;

	static std::vector<Mesh::Vertex> CreateCubeMesh(float size, Block::Type type = Block::Type::dirt);
private:
	Model m_model;
	float m_size;
};