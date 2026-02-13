#pragma once
#include"Resug/Core/Core.h"
#include"Resug/Core/Layer.h"
namespace Resug 
{
	class RESUG_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* overLayer);
		void PopLayer(Layer* layer);
		void PopOverLayer(Layer* overLayer);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		uint m_LayerIndex = 0;

	};
}

