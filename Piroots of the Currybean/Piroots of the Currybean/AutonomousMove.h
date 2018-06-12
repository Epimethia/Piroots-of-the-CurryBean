#pragma once
#include "Resource.h"
#include "Entity.h"

class AutoMove {
public:
	static glm::vec3 Seek(glm::vec3 _ObjPos, glm::vec3 _ObjVelocity, glm::vec3 _Target);

private:
	static glm::vec3 Location;
	static glm::vec3 Velocity;
	static glm::vec3 Acceleration;

	static float MaxSpeed;
	static float MaxForce;
	static float ApproachDistance;

};