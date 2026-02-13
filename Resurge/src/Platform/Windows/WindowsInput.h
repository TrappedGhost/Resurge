#pragma once
#include "Resug/Core/Input.h"
#include"Resug/Core/Application.h"
namespace Resug
{
	class WindowsInput : public Input
	{
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}

