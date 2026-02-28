#include"Sandbox2D.h"

#include"imgui.h"
#include"glm/gtc/type_ptr.hpp"

#include"Platform/OpenGL/OpenGLShader.h"


#define PROFILE_SCOPE(name) Timer time##__LINE__("Sandbox2D::OnUpdate", [&](ProfileResult profileResults) {m_ProfileResults.push_back(profileResults); })

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f/720.0f)
{
}

void Sandbox2D::OnAttach()
{
	
	RG_PROFILE_FUNCTION();
	m_Texture = Resug::Texture2D::Create("Assert/Image/im3_1.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Resug::Timestep& ts)
{
	RG_PROFILE_FUNCTION();

	{
		RG_PROFILE_SCOPE("Camera");
		m_CameraController.OnUpdate(ts);
	}
	{
		RG_PROFILE_SCOPE("Renderer Pre");
		Resug::Renderer2D::ResetStats();
		Resug::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Resug::RendererCommand::Clear();
	}
	{
		RG_PROFILE_SCOPE("Rendering");
		static float rotation = 0.0f;

		rotation += ts *50.0f;
		Resug::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Resug::Renderer2D::DrawRotatedQuad(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f), rotation, glm::vec4(1.0f,0.2f,0.23f,1.0f));
		Resug::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), m_SquareColor);
		Resug::Renderer2D::DrawQuad(glm::vec2(-1.0f, -1.0f), glm::vec2(4.0f, 4.0f), m_Texture, 10.0f, glm::vec4(0.3f,0.3f,0.3f,1.0f));
		Resug::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	RG_PROFILE_FUNCTION();

	ImGui::Begin("Setting");

	auto stats = Resug::Renderer2D::GetStats();
	ImGui::Text("DrawCalls : %d", stats.DrawCalls);
	ImGui::Text("QuadCount : %d", stats.QuadCount);
	ImGui::Text("QuadVertexCount : %d", stats.GetTotalVertexCount());
	ImGui::Text("QuadIndexCount : %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit3("ColorSetting", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Resug::Event& event)
{
	m_CameraController.OnEvent(event);
}
