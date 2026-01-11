#pragma once

#include "Event.h"


namespace Resug
{
	//key event
	class RESUG_API KeyEvent: public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keycode):m_KeyCode(keycode){}
		int m_KeyCode;
	};

	//key pressed event
	class RESUG_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount):KeyEvent(keycode), m_RepeatCount(repeatCount){}
		inline int GetRepeatCount()const { return m_RepeatCount; }
		virtual std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyPressed" << m_KeyCode<<"(repear:"<<m_RepeatCount<<")";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed)
	
	protected:
		int m_RepeatCount;
	};

	//key released event
	class RESUG_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode) :KeyEvent(keycode) {}
		virtual std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyReleased" << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyReleased)

	};

	class RESUG_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode) :KeyEvent(keycode) {}
		virtual std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyTyped" << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyTyped)

	};

}