#include "engine/Cube.h"

Cube::Cube(float size) : 
	Entity(size*size*size, new btBoxShape(btVector3(size/2.f,size / 2.f,size / 2.f))),
	m_size(size),
	m_model(CreateCubeMesh(size))
{

}
void Cube::Update(float delta)
{

}

void Cube::UpdateModels()
{
	btTransform trans;
	rb().getMotionState()->getWorldTransform(trans);
	glm::vec3 euler;
	trans.getRotation().getEulerZYX(euler.z, euler.y, euler.x);
	m_model.SetPosition({
		trans.getOrigin().getX(),
		trans.getOrigin().getY(),
		trans.getOrigin().getZ()
	});
	m_model.SetRotation(euler);
}

void Cube::Draw(const Shader & shader) const
{
	m_model.Draw(shader);
}

Mesh Cube::CreateCubeMesh(float size, Block::Type type)
{
	fRect backRect =  TexturesBlocks::Back(type);
	fRect frontRect = TexturesBlocks::Front(type);
	fRect leftRect = TexturesBlocks::Left(type);
	fRect rightRect = TexturesBlocks::Right(type);
	fRect botRect = TexturesBlocks::Bot(type);
	fRect topRect = TexturesBlocks::Top(type);

	return Mesh( std::vector<Mesh::Vertex>
	{
		//Back
		{ { -size/2.f, -size/2.f, -size/2.f},{ 0.f,0.f,-1.f },{ backRect.x1, backRect.y1 } },//bot
		{ { +size/2.f, +size/2.f, -size/2.f},{ 0.f,0.f,-1.f },{ backRect.x2, backRect.y2 } },
		{ { +size/2.f, -size/2.f, -size/2.f},{ 0.f,0.f,-1.f },{ backRect.x2, backRect.y1 } },
		{ { +size/2.f, +size/2.f, -size/2.f},{ 0.f,0.f,-1.f },{ backRect.x2, backRect.y2 } },//Top
		{ { -size/2.f, -size/2.f, -size/2.f},{ 0.f,0.f,-1.f },{ backRect.x1, backRect.y1 } },
		{ { -size/2.f, +size/2.f, -size/2.f},{ 0.f,0.f,-1.f },{ backRect.x1, backRect.y2 } },

		//Front
		{ { -size/2.f, -size/2.f, +size/2.f},{ 0.f,0.f,1.f },{ frontRect.x1, frontRect.y1 } },//bot
		{ { +size/2.f,-size/2.f, +size/2.f},{ 0.f,0.f,1.f },{ frontRect.x2, frontRect.y1 } },
		{ { +size/2.f, +size/2.f,+size/2.f},{ 0.f,0.f,1.f },{ frontRect.x2, frontRect.y2 } },
		{ { +size/2.f, +size/2.f, +size/2.f},{ 0.f,0.f,1.f },{ frontRect.x2, frontRect.y2 } },//Top
		{ { -size/2.f, +size/2.f, +size/2.f},{ 0.f,0.f,1.f },{ frontRect.x1, frontRect.y2 } },
		{ { -size/2.f, -size/2.f,+size/2.f},{ 0.f,0.f,1.f },{ frontRect.x1, frontRect.y1 } },

		//Left
		{ { -size/2.f, +size/2.f, +size/2.f},{ -1.f,0.f,0.f },{ leftRect.x1, leftRect.y2 } },//Top
		{ { -size/2.f, +size/2.f, -size/2.f},{ -1.f,0.f,0.f },{ leftRect.x2, leftRect.y2 } },
		{ { -size/2.f, -size/2.f, -size/2.f},{ -1.f,0.f,0.f },{ leftRect.x2, leftRect.y1 } },
		{ { -size/2.f, -size/2.f, -size/2.f},{ -1.f,0.f,0.f },{ leftRect.x2, leftRect.y1 } },//Bot
		{ { -size/2.f, -size/2.f, +size/2.f},{ -1.f,0.f,0.f },{ leftRect.x1, leftRect.y1 } },
		{ { -size/2.f, +size/2.f, +size/2.f},{ -1.f,0.f,0.f },{ leftRect.x1, leftRect.y2 } },

		//Right
		{ { +size/2.f, +size/2.f, +size/2.f},{ 1.f,0.f,0.f },{ rightRect.x1, rightRect.y2 } },//Top
		{ { +size/2.f, -size/2.f, -size/2.f},{ 1.f,0.f,0.f },{ rightRect.x2, rightRect.y1 } },
		{ { +size/2.f, +size/2.f, -size/2.f},{ 1.f,0.f,0.f },{ rightRect.x2, rightRect.y2 } },
		{ { +size/2.f, -size/2.f, -size/2.f},{ 1.f,0.f,0.f },{ rightRect.x2, rightRect.y1 } },//Bot
		{ { +size/2.f, +size/2.f, +size/2.f},{ 1.f,0.f,0.f },{ rightRect.x1, rightRect.y2 } },
		{ { +size/2.f, -size/2.f, +size/2.f},{ 1.f,0.f,0.f },{ rightRect.x1, rightRect.y1 } },

		//Bot
		{ { -size/2.f, -size/2.f, -size/2.f},{ 0.f,-1.f,0.f },{ botRect.x1, botRect.y2 } },
		{ { +size/2.f, -size/2.f, -size/2.f},{ 0.f,-1.f,0.f },{ botRect.x2, botRect.y2 } },
		{ { +size/2.f, -size/2.f, +size/2.f},{ 0.f,-1.f,0.f },{ botRect.x2, botRect.y1 } },
		{ { +size/2.f, -size/2.f, +size/2.f},{ 0.f,-1.f,0.f },{ botRect.x2, botRect.y1 } },
		{ { -size/2.f, -size/2.f, +size/2.f},{ 0.f,-1.f,0.f },{ botRect.x1, botRect.y1 } },
		{ { -size/2.f, -size/2.f, -size/2.f},{ 0.f,-1.f,0.f },{ botRect.x1, botRect.y2 } },

		//Top
		{ { +size/2.f,  +size/2.f, -size/2.f},{ 0.f,1.f,0.f },{ topRect.x2, topRect.y2 } },
		{ { -size/2.f,  +size/2.f, -size/2.f},{ 0.f,1.f,0.f },{ topRect.x1, topRect.y2 } },
		{ { +size/2.f,  +size/2.f, +size/2.f},{ 0.f,1.f,0.f },{ topRect.x2, topRect.y1 } },
		{ { -size/2.f,  +size/2.f, +size/2.f},{ 0.f,1.f,0.f },{ topRect.x1, topRect.y1 } },
		{ { +size/2.f,  +size/2.f, +size/2.f},{ 0.f,1.f,0.f },{ topRect.x2, topRect.y1 } },
		{ { -size/2.f,  +size/2.f, -size/2.f},{ 0.f,1.f,0.f },{ topRect.x1, topRect.y2 } }
	});
}



