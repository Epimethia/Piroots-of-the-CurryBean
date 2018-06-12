#include "AutonomousMove.h"
glm::vec3 AutoMove::Location = { 0.0f, 0.0f, 0.0f };
glm::vec3 AutoMove::Velocity = { 0.0f, 0.0f, 0.0f };
glm::vec3 AutoMove::Acceleration = { 0.0f, 0.0f, 0.0f };
float AutoMove::MaxSpeed = 50.0f;
float AutoMove::MaxForce = 1.0f;
float AutoMove::ApproachDistance = 300.0f;
float AutoMove::WanderRadius = 300.0f;

//Name:					Seek
//Parameters:		Object position, Object velocity, Target vector
//Return Type:		glm::vec3 Steering Velocity
//Description:		Takes an objects position and velocity and calculates the
//                             required vector to reach the target point
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

//Name:					Seek (Overload)
//Parameters:		ObjectPos, ObjVelocity, Target, MaxSpeed, MaxForce
//Return Type:		glm::vec3 Steering Velocity
//Description:		Overload of the Seek function to take max speed and max force as well
glm::vec3 AutoMove::Seek(glm::vec3 _ObjPos, glm::vec3 _ObjVelocity, glm::vec3 _Target, float _MaxSpeed, float _MaxForce) {
	Velocity = _ObjVelocity;

	glm::vec3 DesiredVelocity = _Target - _ObjPos;

	float Distance = glm::length(DesiredVelocity);

	DesiredVelocity = glm::normalize(DesiredVelocity) * _MaxSpeed;

	if (Distance < ApproachDistance) DesiredVelocity *= (Distance / ApproachDistance);

	glm::vec3 Steering = DesiredVelocity - _ObjVelocity;

	if (glm::length(Steering) > _MaxForce) Steering = glm::normalize(Steering) * _MaxForce;
	Steering.z = 0.0f;
	return (Steering);
}

//Name:					Persuit
//Parameters:		Object Position, ObjectVelocity
//Return Type:		glm::vec3 Vector to the target
//Description:		Generates a vector towards a point that the input object
//							is travelling towards
glm::vec3 AutoMove::Persue(glm::vec3 _ObjPos, glm::vec3 _ObjVelocity, glm::vec3 _TargetPos, glm::vec3 _TargetVelocity) {
	glm::vec3 Target = _TargetPos;// +(glm::normalize(_TargetVelocity) * 10.0f);
	return Seek(_ObjPos, _ObjVelocity, Target);
}

