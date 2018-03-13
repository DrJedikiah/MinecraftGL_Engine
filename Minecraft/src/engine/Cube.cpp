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

std::vector<Mesh::Vertex> Cube::CreateCubeMesh(float size, Block::Type type, float x, float y, float z)
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
		{ { x-size/2.f, y-size/2.f, z-size/2.f},{ 0.f,0.f,-1.f },{ backRect.x, backRect.y } },//bot
		{ { x+size/2.f, y+size/2.f, z-size/2.f},{ 0.f,0.f,-1.f },{ backRect.x + leftRect.width, backRect.y + leftRect.height } },
		{ { x+size/2.f, y-size/2.f, z-size/2.f},{ 0.f,0.f,-1.f },{ backRect.x + leftRect.width, backRect.y } },
		{ { x+size/2.f, y+size/2.f, z-size/2.f},{ 0.f,0.f,-1.f },{ backRect.x + leftRect.width, backRect.y + leftRect.height } },//Top
		{ { x-size/2.f, y-size/2.f, z-size/2.f},{ 0.f,0.f,-1.f },{ backRect.x, backRect.y } },
		{ { x-size/2.f, y+size/2.f, z-size/2.f},{ 0.f,0.f,-1.f },{ backRect.x, backRect.y + leftRect.height } },

		//Front
		{ { x-size/2.f, y-size/2.f, z+size/2.f},{ 0.f,0.f,1.f },{ frontRect.x, frontRect.y } },//bot
		{ { x+size/2.f, y-size/2.f, z+size/2.f},{ 0.f,0.f,1.f },{ frontRect.x + leftRect.width, frontRect.y } },
		{ { x+size/2.f, y+size/2.f, z+size/2.f},{ 0.f,0.f,1.f },{ frontRect.x + leftRect.width, frontRect.y + leftRect.height } },
		{ { x+size/2.f, y+size/2.f, z+size/2.f},{ 0.f,0.f,1.f },{ frontRect.x + leftRect.width, frontRect.y + leftRect.height } },//Top
		{ { x-size/2.f, y+size/2.f, z+size/2.f},{ 0.f,0.f,1.f },{ frontRect.x, frontRect.y + leftRect.height } },
		{ { x-size/2.f, y-size/2.f, z+size/2.f},{ 0.f,0.f,1.f },{ frontRect.x, frontRect.y } },

		//Left
		{ { x-size/2.f, y+size/2.f, z+size/2.f},{ -1.f,0.f,0.f },{ leftRect.x, leftRect.y + leftRect.height } },//Top
		{ { x-size/2.f, y+size/2.f, z-size/2.f},{ -1.f,0.f,0.f },{ leftRect.x + leftRect.width, leftRect.y + leftRect.height } },
		{ { x-size/2.f, y-size/2.f, z-size/2.f},{ -1.f,0.f,0.f },{ leftRect.x + leftRect.width, leftRect.y } },
		{ { x-size/2.f, y-size/2.f, z-size/2.f},{ -1.f,0.f,0.f },{ leftRect.x + leftRect.width, leftRect.y } },//Bot
		{ { x-size/2.f, y-size/2.f, z+size/2.f},{ -1.f,0.f,0.f },{ leftRect.x, leftRect.y } },
		{ { x-size/2.f, y+size/2.f, z+size/2.f},{ -1.f,0.f,0.f },{ leftRect.x, leftRect.y + leftRect.height } },

		//Right
		{ { x+size/2.f, y+size/2.f, z+size/2.f},{ 1.f,0.f,0.f },{ rightRect.x, rightRect.y + rightRect.height } },//Top
		{ { x+size/2.f, y-size/2.f, z-size/2.f},{ 1.f,0.f,0.f },{ rightRect.x + rightRect.width, rightRect.y } },
		{ { x+size/2.f, y+size/2.f, z-size/2.f},{ 1.f,0.f,0.f },{ rightRect.x + rightRect.width, rightRect.y + rightRect.height } },
		{ { x+size/2.f, y-size/2.f, z-size/2.f},{ 1.f,0.f,0.f },{ rightRect.x + rightRect.width, rightRect.y } },//Bot
		{ { x+size/2.f, y+size/2.f, z+size/2.f},{ 1.f,0.f,0.f },{ rightRect.x, rightRect.y + rightRect.height } },
		{ { x+size/2.f, y-size/2.f, z+size/2.f},{ 1.f,0.f,0.f },{ rightRect.x, rightRect.y } },

		//Bot
		{ { x-size/2.f, y-size/2.f, z-size/2.f},{ 0.f,-1.f,0.f },{ botRect.x, botRect.y + botRect.height } },
		{ { x+size/2.f, y-size/2.f, z-size/2.f},{ 0.f,-1.f,0.f },{ botRect.x + botRect.width, botRect.y + botRect.height } },
		{ { x+size/2.f, y-size/2.f, z+size/2.f},{ 0.f,-1.f,0.f },{ botRect.x + botRect.width, botRect.y } },
		{ { x+size/2.f, y-size/2.f, z+size/2.f},{ 0.f,-1.f,0.f },{ botRect.x + botRect.width, botRect.y } },
		{ { x-size/2.f, y-size/2.f, z+size/2.f},{ 0.f,-1.f,0.f },{ botRect.x, botRect.y } },
		{ { x-size/2.f, y-size/2.f, z-size/2.f},{ 0.f,-1.f,0.f },{ botRect.x, botRect.y + botRect.height } },

		//Top
		{ { x+size/2.f,  y+size/2.f, z-size/2.f},{ 0.f,1.f,0.f },{ topRect.x + topRect.width, topRect.y + topRect.height } },
		{ { x-size/2.f,  y+size/2.f, z-size/2.f},{ 0.f,1.f,0.f },{ topRect.x, topRect.y + topRect.height } },
		{ { x+size/2.f,  y+size/2.f, z+size/2.f},{ 0.f,1.f,0.f },{ topRect.x + topRect.width, topRect.y } },
		{ { x-size/2.f,  y+size/2.f, z+size/2.f},{ 0.f,1.f,0.f },{ topRect.x, topRect.y } },
		{ { x+size/2.f,  y+size/2.f, z+size/2.f},{ 0.f,1.f,0.f },{ topRect.x + topRect.width, topRect.y } },
		{ { x-size/2.f,  y+size/2.f, z-size/2.f},{ 0.f,1.f,0.f },{ topRect.x, topRect.y + topRect.height } }
	};
}



