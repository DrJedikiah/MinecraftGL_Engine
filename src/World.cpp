#include "World.h"

const int World::size = 2;
const int World::height = 1;

World::World()
{
	perlinGen.reseed(42);
	m_chuncks.resize(size * size * height);
	generate();
}

Chunck & World::getChunck(int x, int y, int z)
{
	return m_chuncks[x + z * size + y * size * size];
}

Cube & World::getCube(int x, int y, int z)
{
	return getChunck(x / Chunck::size, y / Chunck::size, z / Chunck::size).GetCube(x%Chunck::size, y%Chunck::size, z%Chunck::size);
}
void World::GeneratePhysics(Physics & physicsEngine)
{
	for (int y = 0; y < height * Chunck::size; ++y)
		for (int x = 0; x < size * Chunck::size; ++x)
			for (int z = 0; z < size * Chunck::size; ++z)
			{
				if (getCube(x, y, z).IsEnabled())
				{
					const float hs = Cube::size / 2.f;
					btTransform transform = btTransform::getIdentity();
					transform.setOrigin(btVector3(x,y,z));
					physicsEngine.CreateRigidBody(0, transform, new btBoxShape(btVector3(hs,hs,hs)));
				}
					
			}	
}

void World::generate()
{	
	//Perlin cubes generation
	for (int y = 0; y < height * Chunck::size; ++y)
		for (int x = 0; x < size * Chunck::size; ++x)
			for (int z = 0; z < size * Chunck::size; ++z)
			{
				const int heightMap = height * Chunck::size;
				float density;
				if (y < heightMap / 100000 )
				{
					float scale3D = 1.f / 5.f;
					density = perlinGen.noise((double)(scale3D * x), (double)(scale3D * y), (double)(scale3D * z));
				}
				else
				{
					float scale2D = 1.f / 10.f;
					int h = heightMap *(perlinGen.noise(scale2D*x, scale2D*z) + 1.f) / 2.f; 
					if (y > h)
						density = -1.f;
					else
						density = 1.f;

				}
				if (density > 0)
					getCube(x, y, z).SetType(CubeType::dirt);
				else
					getCube(x, y, z).SetType(CubeType::air);
			}

	//Removing hidden cubes
	for (int y = 1; y < height * Chunck::size - 1; ++y)
		for (int x = 1; x < size * Chunck::size - 1; ++x)
			for (int z = 1; z < size * Chunck::size  - 1; ++z)
			{
				Cube& cube = getCube(x, y, z);
				if (getCube( x+1,	y,		z	).IsSolid() &&
					getCube( x-1,	y,		z	).IsSolid() &&
					getCube( x,		y+1,	z	).IsSolid() &&
					getCube( x,		y-1,	z	).IsSolid() &&
					getCube( x,		y,		z+1	).IsSolid() &&
					getCube( x,		y,		z-1	).IsSolid())
				{
					cube.SetEnabled(false);
				}

				if (cube.Type() == CubeType::dirt && ! getCube(x, y + 1, z).IsSolid())
					cube.SetType(CubeType::grass);
			}

	//Mesh generation
	for (int x = 0; x < size; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int z = 0; z < size; ++z)
			{
				Chunck & chunck = getChunck(x, y, z);
				chunck.GenerateMesh();
				chunck.m_model.Translate(Chunck::size * Cube::size * glm::vec3{ x,y,z });
			}
		}
	}

}

void World::Draw(const Shader& shader) const
{
	for (const Chunck & chunck : m_chuncks)
		chunck.Draw(shader);
}