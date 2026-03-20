#pragma once
#include"rgpch.h"

#include "Resug/Core/Core.h"

#include"Resug/Core/MouseButtonCode.h"

namespace Resug
{
	class RESUG_API Input
	{
	public:
		 static bool IsKeyPressed(int keycode);
		 static bool IsMouseButtonPressed(int button);
		 static std::pair<float, float> GetMousePosition();
		 static float GetMouseX();
		 static float GetMouseY();

	};
}