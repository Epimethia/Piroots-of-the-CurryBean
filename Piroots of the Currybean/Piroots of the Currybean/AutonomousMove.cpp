#include "AutonomousMove.h"
glm::vec3 AutoMove::Location = { 0.0f, 0.0f, 0.0f };
glm::vec3 AutoMove::Velocity = { 0.0f, 0.0f, 0.0f };
glm::vec3 AutoMove::Acceleration = { 0.0f, 0.0f, 0.0f };
float AutoMove::MaxSpeed = 0.5f;
float AutoMove::MaxForce = 10.0f;


void Limit(glm::vec3& _Vec3, float _float) {
	_Vec3 = glm::normalize(_Vec3) * _float;
}

glm::vec3 AutoMove::Seek(Entity _Obj, glm::vec3 _Target) {
	glm::vec3 DesiredVelocity = _Target - _Obj.GetPos();
	glm::normalize(DesiredVelocity);
	DesiredVelocity *= MaxSpeed;
	Limit(DesiredVelocity, MaxForce);
	return DesiredVelocity - _Obj.GetVelocity();
}
