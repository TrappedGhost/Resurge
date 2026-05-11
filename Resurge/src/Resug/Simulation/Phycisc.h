#pragma once

#include<glm/glm.hpp>

namespace Resug
{
	struct MassPoint
	{
		glm::dvec3 Position;
		glm::dvec3 PrevPosition;
		glm::dvec3 Velocity;
		glm::dvec3 Force;

		float Mass;
		bool Fixed;

		MassPoint(const glm::dvec3& pos, float mass = 1.0f, bool fixed = false)
			: Position(pos), PrevPosition(pos), Velocity(0.0f), Force(0.0f)
			, Mass(mass), Fixed(fixed)
		{
		}
	};
}