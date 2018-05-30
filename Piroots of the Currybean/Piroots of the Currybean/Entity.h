#pragma once
#include "Resource.h"

class Entity3D {
public:
	Entity3D();
	virtual void Draw(GLuint _Program, glm::mat4 _VPMatrix);
	~Entity3D();
protected:
	glm::vec3 ObjPos;
	GLuint VAO;
	GLuint Texture;
	glm::mat4 ModelMatrix;
	glm::mat4 VPMatrix;
};

class CUBE : public Entity3D {
public:
	CUBE();
	CUBE(const char* _TextureRef, float _xPos, float _yPos, float _zPos, float _Scale, float _Rot);
	void Draw(GLuint _Program, glm::mat4 _VPMatrix);
	~CUBE();
};

class PYRAMID : public Entity3D {
public:	
	PYRAMID();
	PYRAMID(const char* _TextureRef, float _xPos, float _yPos, float _zPos, float _Scale, float _Rot);
	void Draw(GLuint _Program, glm::mat4 _VPMatrix);
	~PYRAMID();
};