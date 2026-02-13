#include<Resug.h>

#include"imgui.h"

#include"Platform/OpenGL/OpenGLShader.h"
#include"glm/gtc/type_ptr.hpp"

class ExampleLayer : public Resug::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_CameraController(1280.0f/720.0f)
	{

	}

	virtual void OnAttach()override
	{
		RG_CLIENT_INFO("Example Layer Attach!");
		m_VertexArray.reset(Resug::VertexArray::Create());

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
		};

		std::shared_ptr<Resug::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Resug::VertexBuffer::Create(vertices, sizeof(vertices)));


		Resug::BufferLayout layout =
		{
			{Resug::ShaderDataType::Float3, "a_Position"},
			{Resug::ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indices[3] = { 0,1,2 };
		std::shared_ptr<Resug::IndexBuffer> indexBuffer;
		indexBuffer.reset(Resug::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);
		//---------------------------------------------
		//Square

		m_SquareVA.reset(Resug::VertexArray::Create());
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
			{Resug::ShaderDataType::Float2, "a_TexCoord"}
			});

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0,1,2,2,3,0 };
		std::shared_ptr<Resug::IndexBuffer> squareIB;
		squareIB.reset(Resug::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);


		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
				
			out vec4 v_Color;
			void main()
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
			}
		)";
		std::string fragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec4 v_Color;
			void main()
			{
				color = v_Color;
			}

		)";
		m_Shader = (Resug::Shader::Create("aaa",vertexSrc, fragmentSrc));

		std::string squareVertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;


			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;
			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection *u_Transform* vec4(a_Position, 1.0f);
			}
		)";
		std::string squareFragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			uniform vec3 u_Color;
			uniform sampler2D u_Texture;

			in vec2 v_TexCoord;


			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}

		)";

		//m_FlatShader.reset(Resug::Shader::Create(squareVertexSrc, squareFragmentSrc));
		//m_FlatShader = (Resug::Shader::Create("Texture","Assert/shaders/Texture.glsl"));

		m_ShaderLibary = std::make_shared<Resug::ShaderLibrary>();
		m_ShaderLibary->Load("Texture", "Assert/shaders/Texture.glsl");

		m_Texture2D = Resug::Texture2D::Create("Assert/Image/im3_1.png");
		m_Texture2D_1 = Resug::Texture2D::Create("Assert/Image/im4_2.png");
	}

	virtual void OnUpdate(Resug::Timestep& ts)override
	{
		m_CameraController.OnUpdate(ts);

		Resug::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Resug::RendererCommand::Clear();


		Resug::Renderer::BeginScene(m_CameraController.GetCamera());

		RG_CLIENT_INFO("update");
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(i * 0.11, j*0.11, 0));
				transform = transform * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
				Resug::Renderer::Submit(m_Shader, m_SquareVA, transform);
			}
		}


		auto textureShader = m_ShaderLibary->Get("Texture");

		m_Texture2D->Bind();
		std::dynamic_pointer_cast<Resug::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
		Resug::Renderer::Submit(textureShader, m_SquareVA);

		m_Texture2D_1->Bind();
		std::dynamic_pointer_cast<Resug::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
		Resug::Renderer::Submit(textureShader, m_SquareVA, glm::translate(glm::mat4(1.0f), glm::vec3(0.11, 0.11, 0)));

		Resug::Renderer::EndScene();

	}
	virtual void OnEvent(Resug::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}
	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit3("ColorSetting", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

private:
	Resug::Ref<Resug::ShaderLibrary> m_ShaderLibary;

	Resug::Ref<Resug::Shader> m_Shader;
	Resug::Ref<Resug::VertexArray> m_VertexArray;

	//Resug::Ref<Resug::Shader> m_FlatShader;
	Resug::Ref<Resug::VertexArray> m_SquareVA;
	glm::vec3 m_SquareColor = {0.2,0.3,0.8};

	Resug::Ref<Resug::Texture2D> m_Texture2D;
	Resug::Ref<Resug::Texture2D> m_Texture2D_1;

	Resug::OrthographicCameraController m_CameraController;



};


class Sandbox : public Resug::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		//PushOverLayer(new Resug::ImGuiLayer());
	}
	~Sandbox(){}

private:

};


Resug::Application* Resug::CreateApplication()
{
	return new Sandbox();
}