#pragma once

#include"Event.h"

#include<sstream>

namespace Resug
{
	class RESUG_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint width, uint height):m_Width(width),m_Height(height){}
		inline uint GetWidth() const { return m_Width; }
		inline uint GetHeight()const { return m_Height; }
		virtual std::string ToString()const override
		{
			std::stringstream ss;
			ss << "WindowResize " << m_Width << "," << m_Height;
			return ss.str();
		}
		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		uint m_Width;
		uint m_Height;
	};

	class RESUG_API WindowCloseEvent :public Event
	{
	public:
		WindowCloseEvent(){}
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class RESUG_API AppTickEvent :public Event
	{
	public:
		AppTickEvent() {}
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class RESUG_API AppUpdateEvent :public Event
	{
	public:
		AppUpdateEvent() {}
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class RESUG_API AppRenderEvent :public Event
	{
	public:
		AppRenderEvent() {}
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}