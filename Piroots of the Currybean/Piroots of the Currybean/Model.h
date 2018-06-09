#pragma once

#include "../Dependencies\glew\glew.h"
#include "../Dependencies\freeglut\freeglut.h"
#include "../Dependencies\soil\SOIL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glm\gtc\quaternion.hpp"
#include "glm\gtx\quaternion.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Camera.h"
#include "ModelMesh.h"

class Model {
public:

	GLuint Shader;

	Model();
	/*  Functions   */
	// Constructor, expects a filepath to a 3D model.
	Model(std::string path, GLuint program);

	// Draws the model, and thus all its meshes
	void Render(glm::vec3 _Pos, glm::vec3 _Scale, glm::vec3 _Rot, GLuint _Shader);

private:
	/*  Model Data  */
	std::vector<ModelMesh> meshes;
	std::string directory;
	std::vector<MeshTexture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

	/*  Functions   */
	// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(std::string path);

	// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);

	ModelMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.
	std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	GLint TextureFromFile(const char* path, std::string directory);
};

