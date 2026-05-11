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
	glm::dvec3 MeshCollider2D::OnUpdate(double ts, glm::dvec3* vertexVelocity)
	{

		for (int i = 0; i < m_VertexSize; i++)
		{
			m_VertexVelocity[i] = vertexVelocity[i];
			m_VertexDisplacement[i] = m_VertexVelocity[i] * ts;
			//std::cout << m_VertexVelocity[i] << " ";
		}
		//std::cout << "\n";
		int n = Collider::Colliders.size();
		glm::dvec3 resultDisplacement = glm::dvec3(0.0f);
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
							resultDisplacement = GetResultDisplacement(m_VertexPosition[j] + m_VertexDisplacement[j],
								m_VertexDisplacement[j],
								collider->m_VertexPosition,
								collider->m_VertexSize);
							//if(resultDisplacement.x<0.0f)
							m_VertexDisplacement[j].x = (-0.0005f);
							//m_VertexDisplacement[j].x = -resultDisplacement.x;
							//if (resultDisplacement.x > 0.0f)
								//m_VertexDisplacement[j].x = (0.0005f);
							//m_VertexVelocity[j] = glm::dvec3(0.0f);
							m_VertexVelocity[j].y = -m_VertexVelocity[j].y;
							m_VertexDisplacement[j].y = -m_VertexVelocity[i].y * ts;
							m_VertexDisplacement[j].y = -m_VertexVelocity[i].y * ts;
							//std::cout << m_VertexDisplacement[j] << "\n";
							//m_VertexVelocity[j] = -m_VertexVelocity[j];
						}
					}
				}
			}
		}
		for (int i = 0; i < m_VertexSize; i++)
		{
			if ((m_VertexPosition[i] + m_VertexDisplacement[i]).y < Ground)
			{
				m_VertexDisplacement[i] = glm::dvec3(0.0f);
				//m_VertexVelocity[i] = glm::dvec3(0.0f);
				m_VertexVelocity[i].y = -m_VertexVelocity[i].y ;
				m_VertexDisplacement[i] += m_VertexVelocity[i] * ts;

			}
		}
		for (int i = 0; i < m_VertexSize; i++)
		{
			m_VertexPosition[i] += m_VertexDisplacement[i];
		}


		return glm::dvec3();
	}
}