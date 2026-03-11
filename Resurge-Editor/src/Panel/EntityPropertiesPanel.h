#pragma once


#include"Resug/Scene/Entity.h"

namespace Resug
{

	class EntityPropertiesPanel
	{
	public:
		EntityPropertiesPanel() = default;
		EntityPropertiesPanel(Entity& entity);

		void OnImGuiLayer(Entity entity);

		void DrawComponentToPanel(Entity entity);

	private:
		Entity m_Entity;

	};
}
