#pragma once

#include "Resug/Core/Layer.h"
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
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		//variable
		float m_Time = 0.0f;
	};
}
