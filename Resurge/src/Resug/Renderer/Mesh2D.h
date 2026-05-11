#pragma once

namespace Resug
{

	enum class Mesh2DType
	{
		Quad = 0,
		TriangleLeft = 1,
		TriangleRight = 2,
		Circle = 3,
		Line = 4
	};
	enum class MeshRenderType
	{
		Quad = 0,
		Triangle = 1,
		Line = 2
	};

	class Mesh2D
	{

	public:
		Mesh2D() = default;

		Mesh2D(uint32_t width, uint32_t height, Mesh2DType type);

		Mesh2D(uint32_t width, uint32_t height, Mesh2DType type, MeshRenderType renderType);

		Mesh2D(uint32_t length, uint32_t indexNumber,glm::dvec3 dir, Mesh2DType type, MeshRenderType renderType);

		void CalculateQuadVertexPosition();
		void CalculateLineVertexPosition();

		glm::dvec4* GetVertexPosition() { return m_VertexPosition; }
		const glm::dvec4* GetVertexPosition() const { return m_VertexPosition; }

		void SetVertexPosition(uint32_t x, uint32_t y, glm::dvec4 position) { m_VertexPosition[y * m_Width + x] = position; }
		void SetVertexPosition(uint32_t i, glm::dvec4 position) { m_VertexPosition[i] = position; }

		glm::dvec4 GetVertexPosition(uint32_t x, uint32_t y) {return m_VertexPosition[y * m_Width + x]; }
		glm::dvec4 GetVertexPosition(uint32_t x) {return m_VertexPosition[x]; }

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		Mesh2DType GetType() const { return m_Type; }

		void SetWidth(uint32_t width) { m_Width = width; }
		void SetHeight(uint32_t height) { m_Height = height; }
		void SetType(Mesh2DType type) { m_Type = type; }

		glm::dvec3 GetScale() const { return m_Scale; }
		glm::dvec3 GetRotation() const { return m_Rotation; }

		void SetScale(glm::dvec3 scale) { m_Scale = scale; }
		void SetRotation(glm::dvec3 rotation) { m_Rotation = rotation; }

	public:
		uint32_t m_Width = 3;
		uint32_t m_Height = 3;
		Mesh2DType m_Type = Mesh2DType::Quad;
		MeshRenderType m_RenderType = MeshRenderType::Quad;

		double m_Length = 1.0f;

		glm::dvec3 m_Scale = glm::dvec3(1.0f);
		glm::dvec3 m_Rotation = glm::dvec3(0.0f);

		uint32_t m_TransformVertex = 0;
		glm::dvec4 m_VertexPosition[10000];


	};
}
