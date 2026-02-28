#pragma once

#include"Resug/Core/Core.h"
#include"OrthographicCamera.h"
#include"Shader.h"
#include"RendererAPI.h"
#include"Texture.h"

namespace Resug
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void WindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void Flush();
		static void FlushAndReset();

		static void DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color);
		static void DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color);
		static void DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture, float texZoomLevel = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawQuad(glm::vec3 position, glm::vec2 size, Ref<Texture2D> texture, float texZoomLevel = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(glm::vec2 position, glm::vec2 size,float rotation, glm::vec4 color);
		static void DrawRotatedQuad(glm::vec3 position, glm::vec2 size,float rotation, glm::vec4 color);
		static void DrawRotatedQuad(glm::vec2 position, glm::vec2 size,float rotation, Ref<Texture2D> texture, float texZoomLevel = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(glm::vec3 position, glm::vec2 size,float rotation, Ref<Texture2D> texture, float texZoomLevel = 1.0f, glm::vec4 tintColor = glm::vec4(1.0f));

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
