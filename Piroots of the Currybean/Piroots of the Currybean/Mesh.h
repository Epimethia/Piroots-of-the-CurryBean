#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

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

	//Constructor
	ModelMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<MeshTexture> textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->setupMesh();
	}
	float Rotation = 0.0f;
	float x = 0.0f;
	float y = 0.0f;
	float Radius = 100.0f;
	float CurrentAngle = 0.0f;

	// Render the mesh
	void Render(GLuint& _Program, glm::mat4 _VPMatrix) {
		glUseProgram(_Program);

		//Enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Bind appropriate textures
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		for (GLuint i = 0; i < this->textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);					// Active proper texture unit before binding
			std::stringstream ss;
			std::string number;
			std::string name = this->textures[i].type;
			if (name == "texture_diffuse") ss << diffuseNr++;			// Transfer GLuint to stream
			else if (name == "texture_specular") ss << specularNr++;	// Transfer GLuint to stream
			number = ss.str();

			//Now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(_Program, (name + number).c_str()), i);

			//Bind the Texture
			glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
		}

		glm::mat4 TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(x, y, 0.0f) / 375.0f);

		//Z Rotation
		glm::mat4 RotateZ =
			glm::rotate(
			glm::mat4(),
			glm::radians(0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
			);

		//Y Rotation
		glm::mat4 RotateY =
			glm::rotate(
			glm::mat4(),
			glm::radians(0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
			);

		//X Rotation
		glm::mat4 RotateX =
			glm::rotate(
			glm::mat4(),
			glm::radians(90.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
			);

		glm::mat4 RotationMatrix = RotateX * RotateY * RotateZ;
		glm::mat4 ScaleMatrix = glm::scale(glm::mat4(), glm::vec3(0.1f, 0.1f, 0.1f));
		glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
		glm::mat4 MVP = _VPMatrix * ModelMatrix;

		GLuint  ModelLoc = glGetUniformLocation(_Program, "model");
		glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

		GLuint  MVPLoc = glGetUniformLocation(_Program, "MVP");
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

		//Drawing mesh
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//Disabling backface culling
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);

		//Clearing textures
		for (GLuint i = 0; i < this->textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

private:
	/*  Render data  */
	GLuint VAO, VBO, EBO;

	/*  Functions    */
	// Initializes all the buffer objects/arrays
	void setupMesh() {
		// Create buffers/arrays
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointers
		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
		// Vertex Texture Coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};

class Model {
public:

	GLuint program;

	Model() {};
	/*  Functions   */
	// Constructor, expects a filepath to a 3D model.
	Model(std::string path, GLuint program) {
		this->program = program;
		this->loadModel(path);
	}

	// Draws the model, and thus all its meshes
	void Render(GLuint _Program) {
		for (GLuint i = 0; i < this->meshes.size(); i++) {
			this->meshes[i].Render(_Program, Camera::GetMatrix());
		}
	}

private:
	/*  Model Data  */
	std::vector<ModelMesh> meshes;
	std::string directory;
	std::vector<MeshTexture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

											/*  Functions   */
											// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(std::string path) {
		// Read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		// Check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}
		// Retrieve the directory path of the filepath
		this->directory = path.substr(0, path.find_last_of('/'));

		// Process ASSIMP's root node recursively
		this->processNode(scene->mRootNode, scene);
	}

	// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene) {
		// Process each mesh located at the current node
		for (GLuint i = 0; i < node->mNumMeshes; i++) {
			// The node object only contains indices to index the actual objects in the scene. 
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->processMesh(mesh, scene));
		}


		// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (GLuint i = 0; i < node->mNumChildren; i++) {
			this->processNode(node->mChildren[i], scene);
		}

	}

	ModelMesh processMesh(aiMesh* mesh, const aiScene* scene) {
		// Data to fill
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<MeshTexture> textures;

		// Walk through each of the mesh's vertices
		for (GLuint i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
							  // Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// Normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// Texture Coordinates
			if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			vertices.push_back(vertex);
		}
		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (GLuint i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (GLuint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// Process materials
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			// 1. Diffuse maps
			std::vector<MeshTexture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular maps
			std::vector<MeshTexture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		// Return a mesh object created from the extracted mesh data
		return ModelMesh(vertices, indices, textures);
	}

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.
	std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
		std::vector<MeshTexture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);
			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			GLboolean skip = false;
			for (GLuint j = 0; j < textures_loaded.size(); j++) {
				if (textures_loaded[j].path == str) {
					textures.push_back(textures_loaded[j]);
					skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip) {   // If texture hasn't been loaded already, load it
				MeshTexture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
				this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}

	GLint TextureFromFile(const char* path, std::string directory) {
		//Generate texture ID and load texture data 
		std::string filename = std::string(path);
		filename = directory + '/' + filename;

		std::cout << "texture filename: " << filename << std::endl;

		GLuint textureID;
		glGenTextures(1, &textureID);
		int width, height;
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
		return textureID;
	}
};

