#include "rgpch.h"
#include "Mesh2D.h"

namespace Resug
{
	Mesh2D::Mesh2D(glm::vec4 color, uint32_t width, uint32_t height, Mesh2DType type)
		:m_Color(color), m_Width(width), m_Height(height), m_Type(type)
	{
	}

	void Mesh2D::CalculateVertexPosition()
	{
		float unitWidth = m_Length.x / (m_Width - 1);
		float unitHeight = m_Length.y / (m_Height - 1);

		for (int i = 0; i < m_Height; i++)
		{
			for (int j = 0; j < m_Width; j++)
			{
				m_VertexPosition[i * m_Width + j] = glm::vec4(unitWidth * j, unitHeight * i, 0.0f, 1.0f);
			}
		}
	}

	void Mesh2D::CalculateVertexPositionByRelative(glm::vec3 position)
	{
		for (int i = 0; i < m_Height; i++)
		{
			for (int j = 0; j < m_Width; j++)
			{
				m_VertexPosition[i * m_Width + j] = m_RelativePosition[i * m_Width + j] + glm::vec4(position, 0.0f);
			}
		}
	}

	void Mesh2D::CalculateRelative(glm::vec3 position)
	{
		for (int i = 0; i < m_Height; i++)
		{
			for (int j = 0; j < m_Width; j++)
			{
				m_RelativePosition[i * m_Width + j] = m_VertexPosition[i * m_Width + j] - glm::vec4(position, 0.0f);
			}
		}
	}

	glm::vec4 Mesh2D::CalculateAveragePosition()
	{
		glm::vec4 averagePosition = glm::vec4(0.0f);
		for (int i = 0; i < m_Height; i++)
		{
			for (int j = 0; j < m_Width; j++)
			{
				averagePosition += m_VertexPosition[i * m_Width + j];
			}
		}

		averagePosition /= m_Height * m_Width;

		return averagePosition;
	}
}
