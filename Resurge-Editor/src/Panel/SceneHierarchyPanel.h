#pragma once

#include "Resug.h"

namespace Resug
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene> scene);

		void SetContext(const Ref<Scene> scene);

		Entity GetSeletedEntity() { return m_SelectEntity; }

		void OnImGuiLayer();

		void DrawEntityToPanel(Entity entity);

	private:
		Ref<Scene> m_Scene;
		Entity m_SelectEntity;
	};
}
