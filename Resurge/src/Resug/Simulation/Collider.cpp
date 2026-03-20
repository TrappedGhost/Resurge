#include "rgpch.h"
#include "Collider.h"

namespace Resug
{
	std::vector<BoxCollider2D*> Resug::BoxCollider2D::BoxCollider2Ds;

	BoxCollider2D::BoxCollider2D()
	{
		BoxCollider2Ds.push_back(this);
	}

	glm::vec3 BoxCollider2D::OnUpdate(float ts, glm::vec3 velocity)
	{
		glm::vec3 displacement = velocity * ts;
		for (int i = 0; i < 4; i++)
		{
			m_VertexVelocity[i] = velocity;
			m_VertexDisplacement[i] = m_VertexVelocity[i] * ts;
		}
		int n = BoxCollider2D::BoxCollider2Ds.size();
		if (n > 1)
		{
			for (int i = 0; i < n; i++)
			{
				if (this == BoxCollider2D::BoxCollider2Ds[i])
				{
					
					continue;
				
				}
				for (int j = 0; j < 4; j++)
				{
					//std::cout<<BoxCollider2D::BoxCollider2Ds[i]->m_VertexPosition[j] << " ";
					if (PointInPolygon(m_VertexPosition[j],
						BoxCollider2D::BoxCollider2Ds[i]->m_VertexPosition,
						BoxCollider2D::BoxCollider2Ds[i]->m_VertexSize))
					{
						//std::cout << "?";
						displacement = glm::vec3(0.0f);
					}
				}
			}
		}

		for (int i = 0; i < 4; i++)
		{
			if ((m_VertexPosition[i] + displacement).y < Ground)
			{
				displacement = glm::vec3(0.0f);
			}
		}

		return displacement;
	}
}
