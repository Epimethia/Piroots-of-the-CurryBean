#include "AutonomousMove.h"
glm::vec3 AutoMove::Location = { 0.0f, 0.0f, 0.0f };
glm::vec3 AutoMove::Velocity = { 0.0f, 0.0f, 0.0f };
glm::vec3 AutoMove::Acceleration = { 0.0f, 0.0f, 0.0f };
float AutoMove::MaxSpeed = 50.0f;
float AutoMove::MaxForce = 1.0f;
float AutoMove::ApproachDistance = 300.0f;


glm::vec3 Limit(glm::vec3 _Vec3, float _float) {
	glm::vec3 temp = glm::normalize(_Vec3);
	temp * _float;
	return temp;
}

glm::vec3 AutoMove::Seek(glm::vec3 _ObjPos, glm::vec3 _ObjVelocity, glm::vec3 _Target) {
	//Get the objects current Velocity
	Velocity = _ObjVelocity;

	//Calculated the desired velocity based on the object's position and the target position
	glm::vec3 DesiredVelocity = _Target - _ObjPos;

	//Calculating the distance from the object to the target via finding the length of the DesiredVelocity vect
	float Distance = glm::length(DesiredVelocity);

	//Normalizing
	DesiredVelocity = glm::normalize(DesiredVelocity) * MaxSpeed;

	//If the distance to the target is less than the approach distance (is in "approach" mode)
	if (Distance < ApproachDistance) {
		//Reduce the speed
		DesiredVelocity *= (Distance / ApproachDistance);
	}

	//Calculating the max force that would be applied to the object
	glm::vec3 Steering = DesiredVelocity - _ObjVelocity;

	//Limiting the steering force buy the MaxForce, to make sure the object doesnt turn too quickly
	if (glm::length(Steering) > MaxForce) {
		Steering = glm::normalize(Steering) * MaxForce;
	}

	//Setting the z component to 0.0f as we don't need it  
	Steering.z = 0.0f;

	return (Steering);
}


