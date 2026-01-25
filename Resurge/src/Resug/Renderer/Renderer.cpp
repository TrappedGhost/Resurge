#include"rgpch.h"
#include"Resug/Renderer/Renderer.h"

namespace Resug
{
	Renderer::SceneData* Renderer::m_sceneData = new SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_sceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

	}
	void Renderer::EndScene()
	{

	}
	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", m_sceneData->ViewProjectionMatrix);
		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}
}