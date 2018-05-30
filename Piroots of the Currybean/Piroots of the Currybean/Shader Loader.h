#pragma once

//Resource Header that contains includes
#include "Resource.h"

class ShaderLoader {
private:

	std::string ReadShader(char *filename);
	GLuint CreateShader(GLenum shaderType,
		std::string source,
		char* shaderName);

public:
	ShaderLoader(void);
	~ShaderLoader(void);
	GLuint CreateProgram(char* VertexShaderFilename,
		char* FragmentShaderFilename);

	std::map<std::string, GLuint> Shaders;		//CAUSES MEMORY LEAK


};
