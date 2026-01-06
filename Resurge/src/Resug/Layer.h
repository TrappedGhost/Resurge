#pragma once
#include "Resug/Core.h"
#include"Resug/Event/Event.h"
namespace Resug
{
	class RESUG_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach(){}
		virtual void OnDetach(){}
		virtual void OnUpdate(){}
		virtual void OnEvent(Event& event){}

		inline const std::string& GetName()const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}

