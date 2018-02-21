#include "engine/PlayerAvatar.h"

PlayerAvatar::PlayerAvatar() : 
	Entity(1.f, new btCapsuleShape(0.8f,1.8f)),
	m_modelBody(Cube::CreateCubeMesh(0.1f, Block::Type::player))
{
	rb().setAngularFactor(btVector3(0.f, 1.f, 0.f));
}

void PlayerAvatar::Update(float delta)
{

}

void PlayerAvatar::Draw(const Shader& shader) const
{
	m_modelBody.Draw(shader);
}

void PlayerAvatar::UpdateModels()
{
	btTransform trans;
	rb().getMotionState()->getWorldTransform(trans);
	glm::vec3 euler;
	trans.getRotation().getEulerZYX(euler.z, euler.y, euler.x);
	m_modelBody.SetPosition({
		trans.getOrigin().getX(),
		trans.getOrigin().getY(),
		trans.getOrigin().getZ()
	});
	m_modelBody.SetRotation(euler);

}