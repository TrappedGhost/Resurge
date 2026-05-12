#include "rgpch.h"
#include "BoxCollider2D.h"
#include"CollisionDetection.h"

namespace Resug
{

	BoxCollider2D::BoxCollider2D()
	{
		m_Type = ColliderType::Box2D;
		Collider::Colliders.push_back(this);

		m_Polygon = CreateRef<ConvexPolygon2D>();
		m_Polygon->vertiesPosition = { glm::dvec3(-0.5f, -0.5f, 0.0f), glm::dvec3(0.5f, -0.5f, 0.0f), glm::dvec3(0.5f,  0.5f, 0.0f), glm::dvec3(-0.5f,  0.5f, 0.0f) };
		Geometry2D::Polygons.push_back(m_Polygon);
	}

	glm::dvec3 BoxCollider2D::OnUpdate(double ts, glm::dvec3 velocity)
	{
		bool isCollide = false;

		glm::dvec3 displacement = velocity * ts;
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
				displacement = glm::dvec3(0.0f);
				m_VertexDisplacement[i] = glm::dvec3(0.0f);
				m_VertexVelocity[i] = glm::dvec3(0.0f);
			}
		}

		m_Polygon->vertiesPosition[0] = m_VertexPosition[0];
		m_Polygon->vertiesPosition[1] = m_VertexPosition[1];
		m_Polygon->vertiesPosition[2] = m_VertexPosition[2];
		m_Polygon->vertiesPosition[3] = m_VertexPosition[3];

		return displacement;
	}
}