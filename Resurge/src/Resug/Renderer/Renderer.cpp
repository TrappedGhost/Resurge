#include"rgpch.h"
#include"Resug/Renderer/Renderer.h"
#include"Renderer2D.h"
#include<glm/glm.hpp>
#include"Platform/OpenGL/OpenGLShader.h"
#include "Renderer.h"


namespace Resug
{
	Renderer::SceneData* Renderer::m_sceneData = new SceneData;

	void Renderer::Init()
	{
		RG_PROFILE_FUNCTION();
		RendererCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::WindowResize(uint32_t width, uint32_t height)
	{
		RendererCommand::SetViewport(0, 0, width , height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		RG_PROFILE_FUNCTION();
		m_sceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		glm::mat4 m = m_sceneData->ViewProjectionMatrix;
	}
	void Renderer::EndScene()
	{
		RG_PROFILE_FUNCTION();
	}
	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<Resug::OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_sceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<Resug::OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}
}