#include "rgpch.h"
#include "BoxCollider2D.h"
#include"CollisionDetection.h"

namespace Resug
{

	BoxCollider2D::BoxCollider2D()
	{
		Collider::Colliders.push_back(this);
		m_Type = ColliderType::Box2D;
	}

	glm::vec3 BoxCollider2D::OnUpdate(float ts, glm::vec3 velocity)
	{
		bool isCollide = false;

		glm::vec3 displacement = velocity * ts;
		for (int i = 0; i < 4; i++)
		{
			m_VertexVelocity[i] = velocity;
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
					for (int j = 0; j < 4; j++)
					{
						//std::cout<<BoxCollider2D::BoxCollider2Ds[i]->m_VertexPosition[j] << " ";
						if (PointInPolygon(m_VertexPosition[j],
							collider->m_VertexPosition,
							collider->m_VertexSize))
						{
							isCollide = true;
						}
					}
				}
			}
		}

		for (int i = 0; i < 4; i++)
		{
			if ((m_VertexPosition[i] + displacement).y < Ground)
			{
				isCollide = true;
			}
		}

		if (isCollide)
		{
			for (int i = 0; i < 4; i++)
			{
				displacement = glm::vec3(0.0f);
				m_VertexDisplacement[i] = glm::vec3(0.0f);
				m_VertexVelocity[i] = glm::vec3(0.0f);
			}
		}
		return displacement;
	}
}