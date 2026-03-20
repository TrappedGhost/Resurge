#pragma once

#include"Resug/Core/Core.h"
#include"Camera.h"
#include"OrthographicCamera.h"
#include"Shader.h"
#include"RendererAPI.h"
#include"Texture.h"
#include"SubTexture.h"

namespace Resug
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void WindowResize(uint32_t width, uint32_t height);

		static void InitCircle();
		static void InitTriangle();

		static void BeginScene(Camera& camera, glm::mat4 transform);
		static void BeginScene(glm::mat4 projection, glm::mat4 transform);
		static void BeginScene(OrthographicCamera& camera);
		static void BeginScene(glm::mat4 viewprojection);

		static void EndScene();
		static void Flush();
		static void FlushTriangles();
		
		static void FlushAndReset();

		static void DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color);
		static void DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color);
		static void DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture, float texZoomLevel = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawQuad(glm::vec3 position, glm::vec2 size, Ref<Texture2D> texture, float texZoomLevel = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawQuad(glm::vec2 position, glm::vec2 size, Ref<SubTexture2D> subTexture, float texZoomLevel = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawQuad(glm::vec3 position, glm::vec2 size, Ref<SubTexture2D> subTexture, float texZoomLevel = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));

		static void DrawQuad(glm::mat4 transform, glm::vec4 color);
		static void DrawQuad(glm::mat4 transform, glm::vec4 color, glm::vec4* position);
		static void DrawQuad(glm::mat4 transform, Ref<Texture2D> texture, float texZoomLevel = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawQuad(glm::mat4 transform, Ref<SubTexture2D> subTexture, float texZoomLevel = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		
		//TODO : do mat4 DrawQuad Function...

		static void DrawRotatedQuad(glm::vec2 position, glm::vec2 size,float rotation, glm::vec4 color);
		static void DrawRotatedQuad(glm::vec3 position, glm::vec2 size,float rotation, glm::vec4 color);
		static void DrawRotatedQuad(glm::vec2 position, glm::vec2 size,float rotation, Ref<Texture2D> texture, float texZoomLevel = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(glm::vec3 position, glm::vec2 size,float rotation, Ref<Texture2D> texture, float texZoomLevel = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(glm::vec2 position, glm::vec2 size, float rotation, Ref<SubTexture2D> subTexture, float texZoomLevel = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(glm::vec3 position, glm::vec2 size, float rotation, Ref<SubTexture2D> subTexture, float texZoomLevel = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));


		//Triangle
		struct TriangleProperty
		{
			glm::vec3 Position = glm::vec3 (0.0f);
			glm::vec3 Size = glm::vec3(1.0f);
			glm::vec3 Rotation = glm::vec3(0.0f);
			glm::mat4 Transform = glm::mat4(1.0f);
			glm::vec4 Color = glm::vec4(1.0f);

			uint32_t TextureIndex = 0;
			float TexZoomLevel = 1.0f;
			glm::vec4 TintColor = glm::vec4(1.0f);

			glm::vec3 VertexRelativePosition[3];

			TriangleProperty() = default;
			TriangleProperty(glm::mat4 transform,glm::vec4 color)
				:Transform(transform), Color(color)
			{ }

		};
		static void DrawTriangle(TriangleProperty pro);
		static void DrawTriangle(glm::mat4 transform, glm::vec4 color);
		static void DrawTriangle(glm::mat4 transform, glm::vec4 color, glm::vec4* vertexPosition);


		//Stats;
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static Statistics GetStats();
		static void ResetStats();

	};
}
