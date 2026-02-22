#include "rgpch.h"
#include"RendererCommand.h"
#include "Renderer2D.h"
namespace Resug
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> VA;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

	};

	static Renderer2DStorage* s_Data = new Renderer2DStorage;

	void Renderer2D::Init()
	{
		s_Data->VA = Resug::VertexArray::Create();
		float squareVertices[4 * 5] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 0.0f
		};
		std::shared_ptr<Resug::VertexBuffer> squareVB;
		squareVB.reset(Resug::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{Resug::ShaderDataType::Float3, "a_Position"},
			{Resug::ShaderDataType::Float2, "a_TexCoord	"}
			});

		s_Data->VA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0,1,2,2,3,0 };
		std::shared_ptr<Resug::IndexBuffer> squareIB;
		squareIB.reset(Resug::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		s_Data->VA->SetIndexBuffer(squareIB);


		s_Data->TextureShader =  Shader::Create("FlatColor", "Assert/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->UploadInt("u_Texture", 0);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);

	}
	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}
	void Renderer2D::WindowResize(uint32_t width, uint32_t height)
	{
	}
	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->UploadMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}
	void Renderer2D::EndScene()
	{

	}
	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color)
	{
		Renderer2D::DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}
	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color)
	{
		
		(s_Data->TextureShader)->Bind();
		(s_Data->TextureShader)->UploadFloat4("u_Color", color);
		s_Data->WhiteTexture->Bind();


		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y,1.0f });

		(s_Data->TextureShader)->UploadMat4("u_Transform", transform);

		s_Data->VA->Bind();
		RendererCommand::DrawIndexed(s_Data->VA);
	}

	void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture)
	{
		Renderer2D::DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}
	void Renderer2D::DrawQuad(glm::vec3 position, glm::vec2 size, Ref<Texture2D> texture)
	{
		(s_Data->TextureShader)->Bind();
		(s_Data->TextureShader)->UploadFloat4("u_Color", glm::vec4(1.0f));

		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y,1.0f });
		(s_Data->TextureShader)->UploadMat4("u_Transform", transform);


		s_Data->VA->Bind();
		RendererCommand::DrawIndexed(s_Data->VA);

	}
}