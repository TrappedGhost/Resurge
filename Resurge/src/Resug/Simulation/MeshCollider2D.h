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





		glm::vec3 m_VertexPosition[10000];
		glm::vec3 m_VertexVelocity[10000];
		glm::vec3 m_VertexDisplacement[10000];
		uint32_t m_VertexSize = 0;

		ColliderType m_Type = ColliderType::Box2D;

		bool m_OnGround = false;
	};
}
