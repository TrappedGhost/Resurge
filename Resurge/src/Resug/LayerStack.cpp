#include "rgpch.h"
#include "LayerStack.h"
namespace Resug
{
	LayerStack::LayerStack()
	{
	}
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			delete layer;
		}
	}
	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin(), layer);
		m_LayerIndex++;
	}

	void LayerStack::PushOverLayer(Layer* overLayer)
	{
		m_Layers.emplace_back(overLayer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerIndex--;
		}
	}
	void LayerStack::PopOverLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
			m_Layers.erase(it);
		
	}
}