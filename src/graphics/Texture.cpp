#include "graphics/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "dependencies/stb_image.h"

Texture::Texture(std::string filename)
{
	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	unsigned char *data = stbi_load(filename.c_str(), &m_width, &m_height, &m_nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Texture::Use() const
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

////////////BlockTextureCoords///////////////////////
glm::ivec2 BlockTiles::m_nbTiles = {1,1};
glm::vec2 BlockTiles::m_tileSize = { 1,1 };

std::map<BlockTiles::Block, fRect> BlockTiles::m_blockRectangles;

void BlockTiles::Initialize(int width, int height)
{
	m_nbTiles = glm::ivec2(width, height);
	m_tileSize = glm::vec2(1.f / m_nbTiles.x, 1.f / m_nbTiles.y);
}

void BlockTiles::SetBlockTile(Block block, int x, int y)
{
	m_blockRectangles[block] = {
		m_tileSize.x*x,//X
		m_tileSize.y*y,//Y
		m_tileSize.x*x + m_tileSize.x,//Width
		m_tileSize.y*y + m_tileSize.y//Height
	};
}

fRect BlockTiles::GetRectangle(Block block)
{
	return m_blockRectangles[block];
}

