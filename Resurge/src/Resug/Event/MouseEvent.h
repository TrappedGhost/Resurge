#pragma once

#include"Event.h"

#include<sstream>

namespace Resug
{
	class RESUG_API MouseMoveEvent :public Event
	{
	public:
		MouseMoveEvent(float x, float y) :m_MouseX(x), m_MouseY(y) {}
		inline float GetX() { return m_MouseX; }
		inline float GetY() { return m_MouseY; }
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMove" << m_MouseX << " , " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton)
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
		EVENT_CLASS_CATEGORY(EventCategoryMouseButton|EventCategoryInput)
	protected:
		MouseButtonEvent(int button):m_Button(button){}

		int m_Button;
	};

	class RESUG_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button, int repeatCount):MouseButtonEvent(button),m_RepeatCount(repeatCount){}
		inline int GetRepeatCount()const { return m_RepeatCount; }
		virtual std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressed  " << m_Button << "(RepeatCount:" << m_RepeatCount << ")";
			return ss.str();
		}				 
		EVENT_CLASS_TYPE(MouseButtonPressed)
	
	private:
		int m_RepeatCount;
	};
	class RESUG_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button):MouseButtonEvent(button){}
		virtual std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleased  " << m_Button ;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonReleased)
			 
	};
}
