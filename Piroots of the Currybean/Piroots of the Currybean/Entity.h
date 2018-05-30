#pragma once
#include "Resource.h"

class Entity {
public:
	Entity();
	~Entity();
	virtual void Draw(GLuint _Program, glm::mat4 _VPMatrix);
	glm::vec3 ObjPos;
protected:
	GLuint VAO;
	GLuint Texture;
	glm::mat4 ModelMatrix;
	glm::mat4 VPMatrix;
};

class CUBE : public Entity {
public:
	CUBE();
	CUBE(const char* _TextureRef, float _xPos, float _yPos, float _zPos, float _Scale, float _Rot);
	void Draw(GLuint _Program, glm::mat4 _VPMatrix);
	~CUBE();
};

class PYRAMID : public Entity {
public:	
	PYRAMID();
	PYRAMID(const char* _TextureRef, float _xPos, float _yPos, float _zPos, float _Scale, float _Rot);
	void Draw(GLuint _Program, glm::mat4 _VPMatrix);
	~PYRAMID();
};