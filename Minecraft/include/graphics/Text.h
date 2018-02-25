#pragma once
#include <iostream>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "ft2build.h"
#include FT_FREETYPE_H

#include "graphics/Shader.h"

class Font
{
public:
	Font(std::string filename, int size);
	~Font();

	void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale = 1.f, glm::vec3 color = {1.f,1.f,1.f});



private:
	struct Character {
		GLuint     TextureID;  // ID handle of the glyph texture
		glm::ivec2 Size;       // Size of glyph
		glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
		GLuint     Advance;    // Offset to advance to next glyph
	};
	std::map<GLchar, Character> Characters;

	GLuint VAO, VBO;
}; 

