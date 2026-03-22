#pragma once

namespace Resug
{

	enum class Mesh2DType
	{
		Quad = 0,
		TriangleLeft = 1,
		TriangleRight = 2
	};
	class Mesh2D
	{

	public:
		Mesh2D() = default;
		Mesh2D(glm::vec4 color, uint32_t width, uint32_t height, Mesh2DType type);

		void CalculateVertexPosition();
		void CalculateVertexPositionByRelative(glm::vec3 position);
		void  CalculateRelative(glm::vec3 position);
		void  CalculateRelative(glm::vec4 position);

		glm::vec4 CalculateAveragePosition();


		glm::vec4* GetVertexPosition() { return m_VertexPosition; }
		const glm::vec4* GetVertexPosition() const { return m_VertexPosition; }

		void SetVertexPosition(uint32_t x, uint32_t y, glm::vec4 position) { m_VertexPosition[y * m_Width + x] = position; }
		glm::vec4 GetVertexPosition(uint32_t x, uint32_t y) {return m_VertexPosition[y * m_Width + x]; }
		glm::vec4 GetRelativePosition(uint32_t x, uint32_t y) {return m_RelativePosition[y * m_Width + x]; }

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		Mesh2DType GetType() const { return m_Type; }

		glm::vec2 GetLength() const { return m_Length; }
		glm::vec4 GetColor() const { return m_Color; }

		void SetWidth(uint32_t width) { m_Width = width; }
		void SetHeight(uint32_t height) { m_Height = height; }
		void SetType(Mesh2DType type) { m_Type = type; }

		void SetLength(const glm::vec2& length) { m_Length = length; }
		void SetLength(float x, float y) { m_Length = glm::vec2(x, y); }

		void SetColor(const glm::vec4& color) { m_Color = color; }
		void SetColor(float r, float g, float b, float a) { m_Color = glm::vec4(r, g, b, a); }
	public:
		uint32_t m_Width, m_Height;
		Mesh2DType m_Type;

		glm::vec2 m_Length = glm::vec2(1.0f);

		glm::vec4 m_Color;

		glm::vec4 m_VertexPosition[10000];
		glm::vec4 m_RelativePosition[10000];


	};
}
