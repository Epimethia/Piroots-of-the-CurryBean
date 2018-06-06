#include "CubeMap.h"

//Name:					CubeMap()
//Parameters:		None
//Return Type:		None
//Description:		Default Constructor overload for the
//                             CubeMap class
CubeMap::CubeMap() {};

//Name:					~CubeMap()
//Parameters:		None
//Return Type:		None
//Description:		Default destructor overload for the
//                             CubeMap class
CubeMap::~CubeMap() {};

//Name:					CubeMap()
//Parameters:		std::vector<std::string>
//Return Type:		None
//Description:		Default Constructor overload for the
//                             CubeMap class to create a cubemap with textures
CubeMap::CubeMap(std::vector<std::string> _TextureFilePaths) {
	std::vector<std::string> FilePaths;
	FilePaths.push_back("Right.png");
	FilePaths.push_back("Left.png");
	FilePaths.push_back("Back.png");
	FilePaths.push_back("Front.png");
	FilePaths.push_back("Top.png");
	FilePaths.push_back("Bottom.png");

	//CubeMap vertices (just a cube)
	GLfloat CubeVerts[] = {
		// Positions
		// Front Face            
		-1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,		-1.0f, -1.0f, 1.0f,
		// Right Face            
		1.0f, 1.0f, 1.0f,		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,		1.0f, -1.0f, 1.0f,
		// Back Face             
		1.0f, 1.0f, -1.0f,		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,	1.0f, -1.0f, -1.0f,
		// Left Face             
		-1.0f, 1.0f, -1.0f,		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,		-1.0f, -1.0f, -1.0f,
		// Top Face              
		-1.0f, 1.0f, -1.0f,		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,		-1.0f, 1.0f, 1.0f,
		// Bottom Face           
		-1.0f, -1.0f, 1.0f,		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,		-1.0f, -1.0f, -1.0f
	};

	//Defining Cube Indices
	GLuint CubeIndices[] = {
		0, 1, 2,		0, 2, 3,		// Front Face
		4, 5, 6,		4, 6, 7,		// Right Face
		8, 9, 10,		8, 10, 11,		// Back Face
		12, 13, 14,		12, 14, 15,		// Left Face
		16, 17, 18,		16, 18, 19,		// Top Face
		20, 21, 22,		20, 22, 23,		// Bottom Face
	};

	GLuint VBO;
	GLuint EBO;

	//Generating buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//Binding and setting buffer data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVerts), CubeVerts, GL_STATIC_DRAW);

	//Enabling the positional floats
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Generating EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubeIndices), CubeIndices,
		GL_STATIC_DRAW);

	//Generating and binding the texture
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);

	//For loop to generate textures
	int width, height;
	unsigned char* image;
	for (GLuint i = 0; i < 6; ++i) {
		std::string PathName = CUBEMAP_FILE_PATH;
		PathName.append(FilePaths[i]);
		//Getting the image from filepath
		image = SOIL_load_image(
			PathName.c_str(),
			&width,
			&height,
			0,
			SOIL_LOAD_RGBA
		);

		//Generating the texture from image data
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			image
		);

		//Setting Texture wrap
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 

		//Setting texture filters
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		//Generating mipmaps
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		//Freeing up data
		SOIL_free_image_data(image);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
};

void CubeMap::Render(GLuint _Program, glm::mat4 _VPMatrix) {
	glUseProgram(_Program);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);

	glUniform1i(glGetUniformLocation(_Program, "cubeSampler"), 0);
	glm::mat4 model = glm::scale(glm::mat4(), glm::vec3(10.0f, 10.0f, 10.0f));
	glm::mat4 View = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Projection = glm::perspective(1.0f, 1.0f, 0.1f, 1000.0f);
	glm::mat4 RotationMatrix =
		glm::rotate(
		glm::mat4(),
		glm::radians(-30.0f),
		glm::vec3(1.0f, 0.0f, 0.0f)
		);

	glm::mat4 VPMat = Projection * View * (RotationMatrix) * model;
	glUniformMatrix4fv(glGetUniformLocation(_Program, "MVP"), 1, GL_FALSE, glm::value_ptr(VPMat));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glFrontFace(GL_CW);
}