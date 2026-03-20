#pragma once

#include<glm/glm.hpp>

namespace Resug
{
	struct MassPoint
	{
		glm::vec3 Position;
		glm::vec3 PrevPosition;
		glm::vec3 Velocity;
		glm::vec3 Force;

		float Mass;
		bool Fixed;

		MassPoint(const glm::vec3& pos, float mass = 1.0f, bool fixed = false)
			: Position(pos), PrevPosition(pos), Velocity(0.0f), Force(0.0f)
			, Mass(mass), Fixed(fixed)
		{
		}
	};
}