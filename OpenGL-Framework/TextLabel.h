#pragma once
#include "utils.h"

#include "ShaderLoader.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <string>
#include <iostream>

struct Character
{
	GLuint TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint Advance;
};

class TextLabel
{
public:
	TextLabel(std::string newtext, std::string font, glm::vec2 pos);
	~TextLabel() {}

	void Render();
	
	void SetText(std::string newText) { text = newText; }
	void SetColor(glm::vec3 newColor) { color = newColor; }
	void SetScale(GLfloat newScale) { scale = newScale; }
	void SetPos(glm::vec2 newPos) { position = newPos; }

private:
	std::string text;
	GLfloat scale;
	glm::vec3 color;
	glm::vec2 position;

	GLuint VAO, VBO, m_program;
	std::map<GLchar, Character> Characters;
	static ShaderLoader m_ShaderLoader;
};