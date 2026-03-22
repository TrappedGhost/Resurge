#include "rgpch.h"
#include "MeshCollider2D.h"

#include"CollisionDetection.h"

namespace Resug
{
	MeshCollider2D::MeshCollider2D()
	{
		Collider::Colliders.push_back(this);
		m_Type = ColliderType::Mesh2D;
	}
	glm::vec3 MeshCollider2D::OnUpdate(float ts, glm::vec3* vertexVelocity)
	{

		bool isCollide = false;

		for (int i = 0; i < m_VertexSize; i++)
		{
			m_VertexVelocity[i] = vertexVelocity[i];
			m_VertexDisplacement[i] = m_VertexVelocity[i] * ts;
		}
		int n = Collider::Colliders.size();
		if (n > 1)
		{
			for (int i = 0; i < n; i++)
			{
				if (this == Collider::Colliders[i])
					continue;

				if (Collider::Colliders[i]->m_Type == ColliderType::Box2D)
				{
					BoxCollider2D* collider = (BoxCollider2D*)Collider::Colliders[i];
					for (int j = 0; j < m_VertexSize; j++)
					{

						if (PointInPolygon(m_VertexPosition[j] + m_VertexDisplacement[j],
							collider->m_VertexPosition,
							collider->m_VertexSize))
						{
							isCollide = true;
						}
					}
				}
			}
		}
		for (int i = 0; i < m_VertexSize; i++)
		{
			if ((m_VertexPosition[i] + m_VertexDisplacement[i]).y < Ground)
			{
				isCollide = true;
			}
		}
		if (isCollide)
		{
			for (int i = 0; i < m_VertexSize; i++)
			{
				m_VertexDisplacement[i] = glm::vec3(0.0f);
				m_VertexVelocity[i] = glm::vec3(0.0f);
			}
		}
		else
		{
			for (int i = 0; i < m_VertexSize; i++)
			{
				m_VertexPosition[i] += m_VertexDisplacement[i];
			}
		}

		return glm::vec3();
	}
}