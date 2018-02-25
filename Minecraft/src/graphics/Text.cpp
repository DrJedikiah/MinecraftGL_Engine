#include "graphics/Text.h"

Font::Font(std::string filename, int size)
{
	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, size);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = 
		{
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint)face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}

	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBufferData(GL_ARRAY_BUFFER, 6*4*sizeof(float), nullptr, GL_STATIC_DRAW);
}

void Font::RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	shader.use();
	for (int i = 0; i < (int)text.size(); ++i)
	{
		Character ch = Characters[text[i]];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		GLfloat vertices[] =
		{
			xpos,	ypos,	0, 1,
			xpos+w,	ypos,	1, 1,
			xpos+w,	ypos+h, 1, 0,
			xpos,	ypos,	0, 1,
			xpos+w, ypos+h, 1, 0,
			xpos,	ypos+h, 0, 0
		};
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6) * scale;
	}
}

Font::~Font()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}