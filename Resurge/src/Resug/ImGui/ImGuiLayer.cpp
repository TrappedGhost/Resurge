#include "rgpch.h"
#include "ImGuiLayer.h"

#include"Resug/Application.h"
#include"Platform/OpenGL/ImGioOpenGLRender.h"
#include"GLFW/glfw3.h"
#include"glad/glad.h"
namespace Resug
{
	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer")
	{}

	ImGuiLayer::~ImGuiLayer()
	{

	}
	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImGuiLayer::OnDetach()
	{
	}
	void ImGuiLayer::OnUpdate()
	{

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true; 
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}
	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMoveEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseMoveEvent));
		dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseScrollEvent));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));


	}
	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = true;
		return false;
	}
	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[event.GetMouseButton()] = false;
		return false;
	}
	bool ImGuiLayer::OnMouseMoveEvent(MouseMoveEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(event.GetX(), event.GetY());
		return false;
	}
	bool ImGuiLayer::OnMouseScrollEvent(MouseScrollEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += event.GetXOffset();
		io.MouseWheel  += event.GetYOffset();
		return false;
	}
	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = true;
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		return false;
	}
	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[event.GetKeyCode()] = false;
		return false;
	}
	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = event.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)
		{
			io.AddInputCharacter((uint)keycode);
		}
		return false;
	}
	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(event.GetWidth(), event.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, event.GetWidth(), event.GetHeight());
		return false;
	}
}