#include "AutonomousMove.h"
glm::vec3 AutoMove::Location = { 0.0f, 0.0f, 0.0f };
glm::vec3 AutoMove::Velocity = { 0.0f, 0.0f, 0.0f };
glm::vec3 AutoMove::Acceleration = { 0.0f, 0.0f, 0.0f };
float AutoMove::MaxSpeed = 50.0f;
float AutoMove::MaxForce = 1.5f;


glm::vec3 Limit(glm::vec3 _Vec3, float _float) {
	glm::vec3 temp = glm::normalize(_Vec3);
	temp * _float;
	return temp;
}

glm::vec3 AutoMove::Seek(Entity _Obj, glm::vec3 _Target) {
	Velocity = _Obj.GetVelocity();
	glm::vec3 DesiredVelocity = glm::normalize(_Target - _Obj.GetPos());
	float Distance = glm::length(DesiredVelocity);

	/*if (Distance < 100.0f) {
		DesiredVelocity *= (Distance / 100.0f) * MaxSpeed;
	} else DesiredVelocity * MaxSpeed;*/

	DesiredVelocity *= MaxSpeed;

	glm::vec3 Steering = DesiredVelocity - _Obj.GetVelocity();
	if (glm::length(Steering) > MaxForce) {
		Steering = glm::normalize(Steering) * MaxForce;
	}
	Steering.z = 0.0f;
	return (Steering);
}
