#pragma once

#include"Collider.h"

namespace Resug
{
	class BoxCollider2D :public Collider
	{
	public:
		BoxCollider2D();

		glm::vec3 OnUpdate(float ts, glm::vec3 velocity);




		glm::vec3 m_VertexPosition[4];
		glm::vec3 m_VertexVelocity[4];
		glm::vec3 m_VertexDisplacement[4];
		uint32_t m_VertexSize = 4;


		bool m_OnGround = false;
	};
}
