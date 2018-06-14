#pragma once
#include "Resource.h"
#include "ShaderLoader.h"

struct Character {
	GLuint        TextureID;	// Texture ID 
	glm::ivec2    Size;	// Size of glyph
	glm::ivec2    Bearing;	// Positon of glyph 
	GLuint        Advance;	// How far to move for the next character
};

class Text {
public:
	Text();
	Text(std::string text, std::string font, glm::vec2& pos);
	~Text();

	float HEllo;
	void RenderText();
	void SetPosition(glm::vec2& newPosition) { position = newPosition; };

private:
	std::string text;
	GLfloat scale;
	glm::vec3 color;
	glm::vec2 position;

	GLuint VAO, VBO, TextShaderProg;
	std::map<GLchar, Character> CharMap;
	static ShaderLoader shaderLoader;
};