#include "Text.h"

Text::Text() {}

Text::Text(std::string _Text, std::string _Font, glm::vec2& _Pos) {
	static ShaderLoader shaderLoader;

	text = _Text;
	color = glm::vec3(1.0, 1.0, 1.0);
	scale = 1.0;
	position.x = _Pos.x;
	position.y = _Pos.y;

	//Creating the text shader program with the text VS/FS
	TextShaderProg = shaderLoader.CreateProgram(TEXT_VERT_SHADER, TEXT_FRAG_SHADER);

	//Creating an orthgraphic view, since the text wont really move (it'll be 
	//a part of the UI)
	glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(UTILS::WindowWidth), 0.0f, static_cast<float>(UTILS::WindowHeight));
	glUseProgram(TextShaderProg);
	glUniformMatrix4fv(glGetUniformLocation(TextShaderProg, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

	//Initializing the font library
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) std::cout << "ERROR::FREETYPE: Could not init FreeType Library\n";

	//Loading the font face (what the font looks like
	FT_Face face;
	if (FT_New_Face(ft, _Font.c_str(), 0, &face)) std::cout << "ERROR::FREETYPE: Failed to Load font" << std::endl;

	//Setting the font size to load from the face
	FT_Set_Pixel_Sizes(face, 0, 24);

	//Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Load the first 128 characters
	for (GLubyte c = 0; c < 128; c++) {
		// Load the character glyph into face
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}

		//Generating textures for the character
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,					//Width of character
			face->glyph->bitmap.rows,					//Height of character
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		//Setting texture rendering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Storing the character
		Character ch = { texture,
			glm::ivec2(
				static_cast<GLuint>(face->glyph->bitmap.width),
				static_cast<GLuint>(face->glyph->bitmap.rows)),
			glm::ivec2(
				static_cast<GLuint>(face->glyph->bitmap_left),
				static_cast<GLuint>(face->glyph->bitmap_top)),
			static_cast<GLuint>(face->glyph->advance.x)
		};
		CharMap.insert(std::pair<GLchar, Character>(c, ch));
	}

	//Clearing the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//Clearing the face and font lib
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	//Generating and binding VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Generating and binding VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	//Setting and enabling the vertex attrib pointer
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	//Clearing the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Text::~Text() {
	CharMap.clear();
}

void Text::RenderText() {
	glm::vec2 textPos = position;
	//Enabling culling
	glEnable(GL_CULL_FACE);

	//Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Using the texture program to render the text using the correct shaders
	glUseProgram(TextShaderProg);

	//Sending the texture to the GPU for rendering
	glUniform3f(glGetUniformLocation(TextShaderProg, "textColor"),
				color.x,
				color.y,
				color.z);

	//Setting the texture and VAO to use to render
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	//Iterating through the character map
	for (std::string::const_iterator c = text.begin(); c != text.end(); c++) {
		Character ch = CharMap[*c];

		//Setting the character x/y pos
		GLfloat xpos = textPos.x + ch.Bearing.x * scale;
		GLfloat ypos = textPos.y - (ch.Size.y - ch.Bearing.y) * scale;

		//Setting the character width/height
		GLfloat CharWidth = ch.Size.x * scale;
		GLfloat CharHeight = ch.Size.y * scale;

		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos, ypos + CharHeight, 0.0, 0.0 },
		{ xpos + CharWidth, ypos + CharHeight, 1.0, 0.0 },
		{ xpos + CharWidth, ypos, 1.0, 1.0 },
		{ xpos, ypos + CharHeight, 0.0, 0.0 },
		{ xpos + CharWidth, ypos, 1.0, 1.0 },
		{ xpos, ypos, 0.0, 1.0 },
		};

		//Render the glyph over the quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance cursors for the next glyph
		textPos.x += (ch.Advance >> 6) * scale;
	}

	//Setting vertex array and texture back to null
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Disabling cull and blend
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}