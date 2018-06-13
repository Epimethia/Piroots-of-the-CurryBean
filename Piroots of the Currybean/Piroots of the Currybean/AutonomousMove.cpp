#include "AutonomousMove.h"
glm::vec3 AutoMove::Location = { 0.0f, 0.0f, 0.0f };
glm::vec3 AutoMove::Velocity = { 0.0f, 0.0f, 0.0f };
glm::vec3 AutoMove::Acceleration = { 0.0f, 0.0f, 0.0f };
float AutoMove::MaxSpeed = 50.0f;
float AutoMove::MaxForce = 1.0f;
float AutoMove::ApproachDistance = 300.0f;
float AutoMove::WanderRadius = 300.0f;
float PI = 3.14159265359f;

glm::vec3 AutoMove::Limit(glm::vec3 _Vec, float _MaxForce) {
	glm::vec3 v = _Vec;
	if (glm::length(v) > _MaxForce) {
		v = glm::normalize(v) * _MaxForce;
	}
	return v;
}

glm::vec3 Normalize(glm::vec3 _Vec) {
	if (_Vec.x == 0.0f && _Vec.y == 0.0f && _Vec.z == 0.0f) {
		return glm::vec3();
	}
	else {
		return glm::normalize(_Vec);
	}
}
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
	DesiredVelocity = Normalize(DesiredVelocity) * MaxSpeed;

	//If the distance to the target is less than the approach distance (is in "approach" mode)
	if (Distance < ApproachDistance) {
		//Reduce the speed
		DesiredVelocity *= (Distance / ApproachDistance);
	}

	//Calculating the max force that would be applied to the object
	glm::vec3 Steering = DesiredVelocity - _ObjVelocity;

	//Limiting the steering force buy the MaxForce, to make sure the object doesnt turn too quickly
	/*if (glm::length(Steering) > MaxForce) {
		Steering = glm::normalize(Steering) * MaxForce;
	}*/

	Steering = Limit(Steering, MaxForce);

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

	DesiredVelocity = Normalize(DesiredVelocity) * _MaxSpeed;

	if (Distance < ApproachDistance) DesiredVelocity *= (Distance / ApproachDistance);

	glm::vec3 Steering = DesiredVelocity - _ObjVelocity;

	Steering = Limit(Steering, _MaxSpeed);
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

glm::vec3 AutoMove::Wander(glm::vec3 _ObjPos, glm::vec3 _ObjVelocity) {
	//Calculating a set amount in front of the object
	glm::vec3 ForwardVector = _ObjPos + (Normalize(_ObjVelocity) * 1000.0f);
	
	//Using the x/y values of the forward vector and using parametric calculations, calculate a point on a circle
	glm::vec3 TargetPoint;

	//Finding a random angle (in radians)
	float PiBy180 = PI / 180.0f;
	float Theta = (rand() % 360) * PiBy180;

	//Calculating the x/y with respect to the WanderRadius
	TargetPoint.x = ForwardVector.x + (800.0f * std::cos(Theta));
	TargetPoint.y = ForwardVector.y + (800.0f * std::sin(Theta));

	return Seek(_ObjPos, _ObjVelocity, TargetPoint);
}

glm::vec3 AutoMove::Containment(glm::vec3 _ObjPos, glm::vec3 _ObjVelocity, float _MaxSpeed, float _MaxForce) {
	//Making sure that the autonomous agents remain within the walls of the level
	glm::vec3 DesiredVelocity;
	if (_ObjPos.x >= 3000.0f) {
		DesiredVelocity = { _MaxSpeed, _ObjVelocity.y, 0.0f };
	}
	else if (_ObjPos.x <= -3000.0f) {
		DesiredVelocity = { _MaxSpeed, _ObjVelocity.y, 0.0f };
	}
	else if (_ObjPos.y >= 3000.0f) DesiredVelocity = { _ObjVelocity.x, _MaxSpeed, 0.0f };
	else if (_ObjPos.y <= -3000.0f)	DesiredVelocity = { _ObjVelocity.x, _MaxSpeed, 0.0f };

	glm::vec3 Steering = DesiredVelocity - _ObjVelocity;

	//Limiting the steering force
	Limit(Steering, _MaxForce);

	return Steering;
}

