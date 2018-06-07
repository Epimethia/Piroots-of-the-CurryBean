#pragma once
#include "Resource.h"

class Entity {
public:
	Entity();
	virtual ~Entity();
	virtual glm::vec3& GetPos() { return ObjPos; };
	virtual glm::vec3& GetScale() { return ObjScale; };
	virtual glm::vec3& GetRotation() { return ObjRotation; };
	virtual void Render(GLuint _Program, glm::mat4 _VPMatrix) = 0;


protected:
	GLuint VAO;
	GLuint Texture;

	glm::vec3 ObjPos;
	glm::vec3 ObjScale;
	glm::vec3 ObjRotation;

	glm::mat4 ModelMatrix;
	glm::mat4 VPMatrix;
};

//CUBE DERIVED CLASS
class CUBE : public Entity {
public:
	CUBE();
	CUBE(const char* _TextureRef, glm::vec3 _Pos, glm::vec3 _Scale, glm::vec3 _Rot);
	virtual void Render(GLuint _Program, glm::mat4 _VPMatrix) = 0;
	~CUBE();
};

//PYRAMID DERIVED CLASS
class PYRAMID : public Entity {
public:	
	PYRAMID();
	PYRAMID(const char* _TextureRef, glm::vec3 _Pos, glm::vec3 _Scale, glm::vec3 _Rot);
	void Render(GLuint _Program, glm::mat4 _VPMatrix) = 0;
	~PYRAMID();
};
