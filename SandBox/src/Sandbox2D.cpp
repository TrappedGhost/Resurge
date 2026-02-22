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
		Resug::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Resug::RendererCommand::Clear();
	}

	{
		RG_PROFILE_SCOPE("Rendering");

		Resug::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Resug::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), m_SquareColor);
		Resug::Renderer2D::DrawQuad(glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f), m_SquareColor);
		Resug::Renderer2D::DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(3.0f, 3.0f), m_Texture);
		Resug::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	RG_PROFILE_FUNCTION();

	ImGui::Begin("Setting");
	ImGui::ColorEdit3("ColorSetting", glm::value_ptr(m_SquareColor));
	
	ImGui::End();
}

void Sandbox2D::OnEvent(Resug::Event& event)
{
	m_CameraController.OnEvent(event);
}
