#pragma once

#include "Resug/Layer.h"
#include"Resug/Event/ApplicationEvent.h"
#include"Resug/Event/KeyEvent.h"
#include"Resug/Event/MouseEvent.h"
namespace Resug
{
	class RESUG_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnEvent(Event& event) override;
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
		bool OnMouseMoveEvent(MouseMoveEvent& event);
		bool OnMouseScrollEvent(MouseScrollEvent& event);
		bool OnKeyPressedEvent(KeyPressedEvent& event);
		bool OnKeyReleasedEvent(KeyReleasedEvent& event);
		bool OnKeyTypedEvent(KeyTypedEvent& event);
		bool OnWindowResizeEvent(WindowResizeEvent& event);
		//variable
		float m_Time = 0.0f;
	};
}
