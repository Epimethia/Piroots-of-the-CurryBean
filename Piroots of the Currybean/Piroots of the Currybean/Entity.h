#pragma once
#include "Resource.h"

class Entity {
public:
	Entity();
	~Entity();
	virtual glm::vec3& GetPos() { return ObjPos; };
	virtual glm::vec3& GetScale() { return ObjScale; };
	virtual glm::vec3& GetRotation() { return ObjRotation; };
	virtual void Draw(GLuint _Program, glm::mat4 _VPMatrix);


protected:
	GLuint VAO;
	GLuint Texture;

	glm::vec3 ObjPos;
	glm::vec3 ObjScale;
	glm::vec3 ObjRotation;

	glm::mat4 ModelMatrix;
	glm::mat4 VPMatrix;
};

class CUBE : public Entity {
public:
	CUBE();
	CUBE(const char* _TextureRef, glm::vec3 _Pos, glm::vec3 _Scale, glm::vec3 _Rot);
	//void Draw(GLuint _Program, glm::mat4 _VPMatrix);
	~CUBE();
};

class PYRAMID : public Entity {
public:	
	PYRAMID();
	PYRAMID(const char* _TextureRef, glm::vec3 _Pos, glm::vec3 _Scale, glm::vec3 _Rot);
	void Draw(GLuint _Program, glm::mat4 _VPMatrix);
	~PYRAMID();
};

class WAVEPLANE : public Entity {
public:
	WAVEPLANE();

	~WAVEPLANE();

};

class PLANE : public Entity {
public:	
	PLANE();
	PLANE(const char* _TextureRef, glm::vec3 _Pos, glm::vec3 _Scale, glm::vec3 _Rot);
	void Draw(GLuint _Program, glm::mat4 _VPMatrix);
	~PLANE();
};