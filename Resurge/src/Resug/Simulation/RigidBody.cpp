#include "rgpch.h"
#include "RigidBody.h"

namespace Resug
{
	void RigidBody::CalculateForce()
	{
		Force = glm::dvec3(0.0, -9.8 * Mass, 0.0);
	}
	void RigidBody::CalculateAcceleration()
	{
		CalculateForce();
		Acceleration = Force / Mass;
	}
	void RigidBody::CalculateVelocity(double ts)
	{
		CalculateAcceleration();
		Velocity += Acceleration * ts;
	}
	glm::dvec3 RigidBody::GetDisplacement(double ts)
	{
		CalculateForce();
		CalculateAcceleration();
		CalculateVelocity(ts);
		return Velocity * ts;
	}
}
