#include "rgpch.h"
#include "Mesh2D.h"

namespace Resug
{
	Mesh2D::Mesh2D(uint32_t width, uint32_t height, Mesh2DType type)
		: m_Width(width), m_Height(height), m_Type(type)
	{
		if (type == Mesh2DType::Quad)
		{
			CalculateQuadVertexPosition();
		}
	}

	Mesh2D::Mesh2D( uint32_t width, uint32_t height, Mesh2DType type, MeshRenderType renderType)
		: m_Width(width), m_Height(height), m_Type(type), m_RenderType(renderType)
	{
		if (type == Mesh2DType::Quad)
		{
			CalculateQuadVertexPosition();
		}
		if (type == Mesh2DType::Line)
		{
			m_Length = 3.0f;
			for (int i = 0; i < m_Height; i++)
			{
				m_VertexPosition[i] = glm::dvec4(0.0f, float(i) / float(m_Height - 1) * m_Length, 0.0f, 1.0f);
			}
		}
	}

	Mesh2D::Mesh2D(uint32_t length, uint32_t indexNumber, glm::dvec3 dir, Mesh2DType type, MeshRenderType renderType)
		: m_Width(1), m_Height(indexNumber), m_Type(type), m_RenderType(renderType)
	{
		if (type == Mesh2DType::Line)
		{
			m_Length = length;

			for (int i = 0; i < indexNumber; i++)
			{
				m_VertexPosition[i] = glm::dvec4(dir * (double(i) / double(indexNumber - 1)) * m_Length, 1.0f);
			}
		}
	}

	void Mesh2D::CalculateQuadVertexPosition()
	{
		float unitWidth = 1.0f / (m_Width - 1);
		float unitHeight = 1.0f / (m_Height - 1);

		for (int i = 0; i < m_Height; i++)
		{
			for (int j = 0; j < m_Width; j++)
			{
				m_VertexPosition[i * m_Width + j] = glm::dvec4(unitWidth * j, unitHeight * i, 0.0f, 1.0f);
			}
		}
	}

}
