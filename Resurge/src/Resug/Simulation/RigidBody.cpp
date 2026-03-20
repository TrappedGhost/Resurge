#include "rgpch.h"
#include "RigidBody.h"

namespace Resug
{
	void RigidBody::CalculateForce()
	{
		Force = glm::vec3(0.0f, -9.8f * Mass, 0.0f);
	}
	void RigidBody::CalculateAcceleration()
	{
		CalculateForce();
		Acceleration = Force / Mass;
	}
	void RigidBody::CalculateVelocity(float ts)
	{
		CalculateAcceleration();
		Velocity += Acceleration * ts;
	}
	glm::vec3 RigidBody::GetDisplacement(float ts)
	{
		CalculateForce();
		CalculateAcceleration();
		CalculateVelocity(ts);
		return Velocity * ts;
	}
}
