#pragma once

#include"Collider.h"

namespace Resug
{
	class MeshCollider2D :public Collider
	{
	public:
		MeshCollider2D();

		glm::dvec3 OnUpdate(double ts, glm::dvec3* vertexVelocity);

		void SetVertexSize(uint32_t size) { m_VertexSize = size; }

		glm::dvec3 GetVertexPosition(uint32_t x) { return m_VertexPosition[x]; }




		glm::dvec3 m_VertexPosition[10000];
		glm::dvec3 m_VertexVelocity[10000];
		glm::dvec3 m_VertexDisplacement[10000];
		uint32_t m_VertexSize = 0;

		bool m_OnGround = false;
	};
}
