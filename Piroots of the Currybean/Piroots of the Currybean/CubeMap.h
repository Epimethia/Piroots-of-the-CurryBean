#pragma once
#include "Resource.h"
#include "Camera.h"

class CubeMap {
public:
	CubeMap();
	~CubeMap();
	CubeMap(std::vector<std::string> _TextureFilePaths);
	void Render(GLuint _Program, glm::mat4 _VPMatrix);

private:
	GLuint VAO;
	GLuint Texture;
};