#include "engine/map/Chunck.h"

const int seed = 33;
PerlinNoise Chunck::perlinGen(seed);

Chunck::Chunck(int x, int z) :
	m_positionX(x),
	m_positionZ(z),
	m_enabled(true)
{ 
	for (int y = 0; y < Chunck::height; ++y)
		m_subChuncks[y] = new SubChunck(glm::ivec3(x, y, z), this);
}

Block* Chunck::GetBlock(glm::ivec3 position)
{
	if(position.y < 0 || position.y >= Chunck::height * SubChunck::size)
		return nullptr;

	SubChunck * subChunck = m_subChuncks[position.y / SubChunck::size];
	if (subChunck)
		return subChunck->GetBlock( glm::ivec3( position.x, position.y % SubChunck::size, position.z));
	else
		return nullptr;
}

SubChunck*  Chunck::GetSubChunck(int  height)
{
	if (height < 0 || height >= Chunck::height)
		return nullptr;
	else
		return m_subChuncks[height];
}

void Chunck::Update(float delta)
{
	for (int y = 0; y < Chunck::height; ++y)
		m_subChuncks[y]->Update(delta);
}

void Chunck::GenerateLater(int subChunck)	
{ 
	if (subChunck >= 0 && subChunck < Chunck::height)
		m_subChuncks[subChunck]->m_regenerateLater = true;
}

void Chunck::GenerateBlocks()	
{
	//Set stone
	for (int x = 0; x < SubChunck::size; ++x)
		for (int y = 0; y < SubChunck::size * Chunck::height; ++y)
			for (int z = 0; z < SubChunck::size; ++z)
			{
				glm::vec3 pos = glm::vec3( m_positionX *SubChunck::size + (float)x, (float)y, m_positionZ *SubChunck::size + (float)z);

				//////////////////
				float freq3D1 = 30.f;
				float density3D1 = 1.f - 0.5f * (1.f + (float)Chunck::perlinGen.noise(pos.x / freq3D1, pos.y / freq3D1, pos.z / freq3D1));
				float freq3D2 = 10.f;
				float density3D2 = 1.f - 0.5f * (1.f + (float)Chunck::perlinGen.noise(pos.x / freq3D2, pos.y / freq3D2, pos.z / freq3D2));
				
				float density3D = 0.8f * density3D1 + 0.2f*density3D2;

				
				///////////
				float freq2D1 = 150.f;
				float perlin2D1 = 0.5f * (1.f + (float)Chunck::perlinGen.noise(pos.x / freq2D1, pos.z / freq2D1));
				float freq2D2 = 50.f;
				float perlin2D2 = 0.5f * (1.f + (float)Chunck::perlinGen.noise(pos.x / freq2D2, pos.z / freq2D2));
				float perlin2D = 0.7f * perlin2D1 + 0.3f*perlin2D2;

				//////////
				float density2D = 1.f + perlin2D - pos.y / (SubChunck::size * Chunck::height);
				
				
				float density = 0.3f * density3D + 0.7f* density2D* density2D;

				if (density > 0.7f)
					GetBlock( glm::ivec3(x,y,z) )->SetType(Block::Type::stone);
				else
					GetBlock(glm::ivec3(x, y, z))->SetType(Block::Type::air);

			}
	
	//Set dirt
	for (int x = 0; x < SubChunck::size; ++x)
		for (int y = 0; y < SubChunck::size * Chunck::height; ++y)
			for (int z = 0; z < SubChunck::size; ++z)
			{
				glm::ivec3 pos = glm::ivec3(SubChunck::size * m_positionX + x, y, SubChunck::size * m_positionZ +  z);

				int nbDirt = (int)(10.f * ( 1.f - (float)pos.y / (SubChunck::size * Chunck::height)));

				Block* otherBlock = GetBlock(glm::ivec3(x, y+1, z));
				Block* block = GetBlock( glm::ivec3(x, y, z));

				if (block->type == Block::Type::stone && (!otherBlock || !otherBlock->solid))
					for (int i = 0; i < (int)nbDirt; ++i)
					{
						Block* blockDirt = GetBlock(glm::ivec3(x, y - i, z));
						if (blockDirt && blockDirt->type == Block::Type::stone)
							blockDirt->SetType(Block::Type::dirt);
					}
			}

	//Set caves
	for (int x = 0; x < SubChunck::size; ++x)
		for (int y = 0; y < SubChunck::size * Chunck::height; ++y)
			for (int z = 0; z < SubChunck::size; ++z)
			{
				glm::ivec3 pos = glm::ivec3(SubChunck::size * m_positionX + x, y, SubChunck::size * m_positionZ + z);

				float hratio = (float)pos.y / (SubChunck::size * Chunck::height);

				float cavesFreqLow = 15;
				float cavesLow = 0.5f *(1.f + (float)Chunck::perlinGen.noise(pos.x / cavesFreqLow, pos.y / cavesFreqLow, pos.z / cavesFreqLow));
				float cavesFreqHigh = cavesFreqLow/3;
				float cavesHigh = 0.5f *(1.f + (float)Chunck::perlinGen.noise(pos.x / cavesFreqHigh, pos.y / cavesFreqHigh, pos.z / cavesFreqHigh));

				//cavesDensity
				float cavesDensity = 0.8f*cavesLow + 0.2f*cavesHigh;

				//Set blocks
				if (cavesDensity < 0.4 * ( 1.f - 2 * pow(hratio,3))   )
					GetBlock(glm::ivec3(x, y, z))->SetType(Block::Type::air);
			}

	//Set grass	
	for (int x = 0; x < SubChunck::size; ++x)
		for (int y = 0; y < SubChunck::size * Chunck::height; ++y)
			for (int z = 0; z < SubChunck::size; ++z)
			{
				glm::ivec3 pos = glm::ivec3(SubChunck::size * m_positionX + x, y, SubChunck::size * m_positionZ + z);

				Block* otherBlock = GetBlock(glm::ivec3(x, y + 1, z));
				if (GetBlock(glm::ivec3(x, y, z))->type == Block::Type::dirt && (!otherBlock || !otherBlock->solid))

					GetBlock(glm::ivec3(x, y, z))->SetType(Block::Type::grass);
			}

	//Set bedrock
	for (int x = 0; x < SubChunck::size; ++x)
		for (int y = 0; y < 5; ++y)
			for (int z = 0; z < SubChunck::size; ++z)
			{
				glm::ivec3 pos = glm::ivec3(SubChunck::size * m_positionX + x, y, SubChunck::size * m_positionZ + z);
				GetBlock(glm::ivec3(x, y, z))->SetType(Block::Type::bedrock);
			}

	m_blocksGenerated = true;

	for (int y = 0; y < Chunck::height; ++y)
		m_subChuncks[y]->CheckEmpty();

}
void Chunck::GenerateMesh( int subChunck )
{ 
	if (m_blocksGenerated)
		m_subChuncks[subChunck]->GenerateMesh();
	else
		std::cerr << "ERROR: Chunck::GenerateMesh blocks not generated" << std::endl;
}

void Chunck::GenerateModels(int subChunck)
{
	if (m_blocksGenerated)
		m_subChuncks[subChunck]->GenerateModels();
	else
		std::cerr << "ERROR: Chunck::GenerateModels blocks not generated" << std::endl;
}

void Chunck::GenerateCollider( int subChunck, bool regenerate)
{
	if (subChunck >= 0 && subChunck < Chunck::height)
		if (!m_subChuncks[subChunck]->m_colliderGenerated || regenerate)
			 m_subChuncks[subChunck]->GenerateCollider();
}

void Chunck::DrawTransparent(const Shader & shader) const
{
	if (m_enabled)
		for (int y = 0; y <Chunck::height; ++y)
			m_subChuncks[y]->DrawTransparent(shader);
}

void Chunck::DrawOpaque(const Shader & shader) const
{
	if (m_enabled)
		for (int y = 0; y <Chunck::height; ++y)
			m_subChuncks[y]->DrawOpaque(shader);
}

void Chunck::SetEnabled(bool state)
{
	m_enabled = state;
	for (int y = 0; y < Chunck::height; ++y)
		m_subChuncks[y]->SetEnabled(state);
}

void Chunck::SetSubChunckEnabled(int subChunck, bool state)
{
	m_subChuncks[subChunck]->SetEnabled(state);
}

bool Chunck::Enabled() const { return m_enabled; }
bool Chunck::BlocksGenerated() const { return m_blocksGenerated; }


glm::ivec3 Chunck::Position() const{return glm::ivec3(m_positionX,0, m_positionZ);}

Chunck::~Chunck()
{
	for (int y = 0; y <Chunck::height; ++y)
		delete m_subChuncks[y];
}


