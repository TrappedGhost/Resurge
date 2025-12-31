#pragma once

#include"Event.h"

#include<sstream>

namespace Resug 
{
	class RESUG_API MouseMoveEvent :public Event
	{
	public:
		MouseMoveEvent(float x, float y):m_MouseX(x),m_MouseY(y){}
		inline float GetX() { return m_MouseX;}
		inline float GetY() { return m_MouseY;}
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMove" << m_MouseX << " , "<<m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse| EventCategoryMouseButton)
	private:
		float m_MouseX;
		float m_MouseY;
	};

	class RESUG_API MouseScrollEvent :public Event
	{
	public:
		MouseScrollEvent(float xOffset, float yOffset) {}
		inline float GetXOffset() { return m_XOffset; }
		inline float GetYOffset() { return m_YOffset; }
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMove" << m_XOffset << " , " << m_YOffset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton)
	private:
		float m_XOffset;
		float m_YOffset;

	};

	class RESUG_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; }
	
	};
}