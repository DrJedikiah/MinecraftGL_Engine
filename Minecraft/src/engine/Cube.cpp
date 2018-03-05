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

std::vector<Mesh::Vertex> Cube::CreateCubeMesh(float size, Block::Type type)
{
	fRect backRect =  TexturesBlocks::Back(type);
	fRect frontRect = TexturesBlocks::Front(type);
	fRect leftRect = TexturesBlocks::Left(type);
	fRect rightRect = TexturesBlocks::Right(type);
	fRect botRect = TexturesBlocks::Bot(type);
	fRect topRect = TexturesBlocks::Top(type);

	return std::vector<Mesh::Vertex>
	{
		//Back
		{ { -size/2.f, -size/2.f, -size/2.f},{ 0.f,0.f,-1.f },{ backRect.x, backRect.y } },//bot
		{ { +size/2.f, +size/2.f, -size/2.f},{ 0.f,0.f,-1.f },{ backRect.x + leftRect.width, backRect.y + leftRect.height } },
		{ { +size/2.f, -size/2.f, -size/2.f},{ 0.f,0.f,-1.f },{ backRect.x + leftRect.width, backRect.y } },
		{ { +size/2.f, +size/2.f, -size/2.f},{ 0.f,0.f,-1.f },{ backRect.x + leftRect.width, backRect.y + leftRect.height } },//Top
		{ { -size/2.f, -size/2.f, -size/2.f},{ 0.f,0.f,-1.f },{ backRect.x, backRect.y } },
		{ { -size/2.f, +size/2.f, -size/2.f},{ 0.f,0.f,-1.f },{ backRect.x, backRect.y + leftRect.height } },

		//Front
		{ { -size/2.f, -size/2.f, +size/2.f},{ 0.f,0.f,1.f },{ frontRect.x, frontRect.y } },//bot
		{ { +size/2.f,-size/2.f, +size/2.f},{ 0.f,0.f,1.f },{ frontRect.x + leftRect.width, frontRect.y } },
		{ { +size/2.f, +size/2.f,+size/2.f},{ 0.f,0.f,1.f },{ frontRect.x + leftRect.width, frontRect.y + leftRect.height } },
		{ { +size/2.f, +size/2.f, +size/2.f},{ 0.f,0.f,1.f },{ frontRect.x + leftRect.width, frontRect.y + leftRect.height } },//Top
		{ { -size/2.f, +size/2.f, +size/2.f},{ 0.f,0.f,1.f },{ frontRect.x, frontRect.y + leftRect.height } },
		{ { -size/2.f, -size/2.f,+size/2.f},{ 0.f,0.f,1.f },{ frontRect.x, frontRect.y } },

		//Left
		{ { -size/2.f, +size/2.f, +size/2.f},{ -1.f,0.f,0.f },{ leftRect.x, leftRect.y + leftRect.height } },//Top
		{ { -size/2.f, +size/2.f, -size/2.f},{ -1.f,0.f,0.f },{ leftRect.x + leftRect.width, leftRect.y + leftRect.height } },
		{ { -size/2.f, -size/2.f, -size/2.f},{ -1.f,0.f,0.f },{ leftRect.x + leftRect.width, leftRect.y } },
		{ { -size/2.f, -size/2.f, -size/2.f},{ -1.f,0.f,0.f },{ leftRect.x + leftRect.width, leftRect.y } },//Bot
		{ { -size/2.f, -size/2.f, +size/2.f},{ -1.f,0.f,0.f },{ leftRect.x, leftRect.y } },
		{ { -size/2.f, +size/2.f, +size/2.f},{ -1.f,0.f,0.f },{ leftRect.x, leftRect.y + leftRect.height } },

		//Right
		{ { +size/2.f, +size/2.f, +size/2.f},{ 1.f,0.f,0.f },{ rightRect.x, rightRect.y + rightRect.height } },//Top
		{ { +size/2.f, -size/2.f, -size/2.f},{ 1.f,0.f,0.f },{ rightRect.x + rightRect.width, rightRect.y } },
		{ { +size/2.f, +size/2.f, -size/2.f},{ 1.f,0.f,0.f },{ rightRect.x + rightRect.width, rightRect.y + rightRect.height } },
		{ { +size/2.f, -size/2.f, -size/2.f},{ 1.f,0.f,0.f },{ rightRect.x + rightRect.width, rightRect.y } },//Bot
		{ { +size/2.f, +size/2.f, +size/2.f},{ 1.f,0.f,0.f },{ rightRect.x, rightRect.y + rightRect.height } },
		{ { +size/2.f, -size/2.f, +size/2.f},{ 1.f,0.f,0.f },{ rightRect.x, rightRect.y } },

		//Bot
		{ { -size/2.f, -size/2.f, -size/2.f},{ 0.f,-1.f,0.f },{ botRect.x, botRect.y + botRect.height } },
		{ { +size/2.f, -size/2.f, -size/2.f},{ 0.f,-1.f,0.f },{ botRect.x + botRect.width, botRect.y + botRect.height } },
		{ { +size/2.f, -size/2.f, +size/2.f},{ 0.f,-1.f,0.f },{ botRect.x + botRect.width, botRect.y } },
		{ { +size/2.f, -size/2.f, +size/2.f},{ 0.f,-1.f,0.f },{ botRect.x + botRect.width, botRect.y } },
		{ { -size/2.f, -size/2.f, +size/2.f},{ 0.f,-1.f,0.f },{ botRect.x, botRect.y } },
		{ { -size/2.f, -size/2.f, -size/2.f},{ 0.f,-1.f,0.f },{ botRect.x, botRect.y + botRect.height } },

		//Top
		{ { +size/2.f,  +size/2.f, -size/2.f},{ 0.f,1.f,0.f },{ topRect.x + topRect.width, topRect.y + topRect.height } },
		{ { -size/2.f,  +size/2.f, -size/2.f},{ 0.f,1.f,0.f },{ topRect.x, topRect.y + topRect.height } },
		{ { +size/2.f,  +size/2.f, +size/2.f},{ 0.f,1.f,0.f },{ topRect.x + topRect.width, topRect.y } },
		{ { -size/2.f,  +size/2.f, +size/2.f},{ 0.f,1.f,0.f },{ topRect.x, topRect.y } },
		{ { +size/2.f,  +size/2.f, +size/2.f},{ 0.f,1.f,0.f },{ topRect.x + topRect.width, topRect.y } },
		{ { -size/2.f,  +size/2.f, -size/2.f},{ 0.f,1.f,0.f },{ topRect.x, topRect.y + topRect.height } }
	};
}



