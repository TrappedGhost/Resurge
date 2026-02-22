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

		static void DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color);
		static void DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color);
		static void DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture);
		static void DrawQuad(glm::vec3 position, glm::vec2 size, Ref<Texture2D> texture);

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_sceneData;
		Ref<Texture2D> m_WhiteTexture;

	};
}
