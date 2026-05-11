#pragma once

#include"Collider.h"

namespace Resug
{
	class BoxCollider2D :public Collider
	{
	public:
		BoxCollider2D();

		glm::dvec3 OnUpdate(double ts, glm::dvec3 velocity);




		glm::dvec3 m_VertexPosition[4];
		glm::dvec3 m_VertexVelocity[4];
		glm::dvec3 m_VertexDisplacement[4];
		uint32_t m_VertexSize = 4;


		bool m_OnGround = false;
	};
}
