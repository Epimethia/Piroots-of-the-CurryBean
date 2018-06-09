#pragma once

#include "Resource.h"
#include <fstream>
#include <sstream>

#include "glm\gtc\quaternion.hpp"
#include "glm\gtx\quaternion.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

struct Vertex {
	glm::vec3 Position; // Position	
	glm::vec3 Normal; // Normal	
	glm::vec2 TexCoords; // TexCoords
};

struct MeshTexture {
	GLuint id;
	std::string type;
	aiString path;
};

class ModelMesh {
public:
	//Mesh Data
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<MeshTexture> textures;

	GLuint ModelShader;

	//Constructor
	ModelMesh();
	ModelMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<MeshTexture> textures);

	// Render the mesh
	void Render(glm::mat4 _VPMatrix, glm::vec3 Pos, glm::vec3 Scale, glm::vec3 Rot, GLuint _Shader);

private:

	GLuint VAO, VBO, EBO;

	void setupMesh();
};