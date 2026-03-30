#pragma once

#include"Collider.h"

namespace Resug
{
	class MeshCollider2D :public Collider
	{
	public:
		MeshCollider2D();

		glm::vec3 OnUpdate(float ts, glm::vec3* vertexVelocity);

		void SetVertexSize(uint32_t size) { m_VertexSize = size; }

		glm::vec3 GetVertexPosition(uint32_t x) { return m_VertexPosition[x]; }




		glm::vec3 m_VertexPosition[10000];
		glm::vec3 m_VertexVelocity[10000];
		glm::vec3 m_VertexDisplacement[10000];
		uint32_t m_VertexSize = 0;

		bool m_OnGround = false;
	};
}
