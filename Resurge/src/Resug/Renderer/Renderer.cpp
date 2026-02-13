#include"rgpch.h"
#include"Resug/Renderer/Renderer.h"

#include<glm/glm.hpp>
#include"Platform/OpenGL/OpenGLShader.h"


namespace Resug
{
	Renderer::SceneData* Renderer::m_sceneData = new SceneData;

	void Renderer::Init()
	{
		RendererCommand::Init();
	}

	void Renderer::WindowResize(uint32_t width, uint32_t height)
	{
		RendererCommand::SetViewport(0, 0, width , height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_sceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		glm::mat4 m = m_sceneData->ViewProjectionMatrix;
		RG_CORE_WARN("{0},{1},{2},{3}", m[0][0], m[0][1], m[0][2], m[0][3]);
	}
	void Renderer::EndScene()
	{

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