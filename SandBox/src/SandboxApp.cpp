#include<Resug.h>

#include"imgui.h"

class ExampleLayer : public Resug::Layer
{
public:
	ExampleLayer() :Layer("Example")
	{
	}

	virtual void OnUpdate()override
	{
		//RG_CLIENT_INFO("ExampleLayer::Update");
		if (Resug::Input::IsKeyPressed(RG_KEY_TAB))
		{
			//RG_CLIENT_INFO("tab is pressed");
		}
	}
	virtual void OnEvent(Resug::Event& event) override
	{
		if (event.GetEventType() == Resug::EventType::KeyPressed)
		{
			Resug::KeyPressedEvent& e = (Resug::KeyPressedEvent&)event;
			if (e.GetKeyCode() == RG_KEY_TAB)
			{
				RG_CLIENT_TRACE("TAB is pressed");
			}
		}
	}
	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Tesr");
		ImGui::Text("Hello World");
		ImGui::End();
	}

private:

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