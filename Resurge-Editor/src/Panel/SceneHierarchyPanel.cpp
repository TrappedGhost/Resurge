#include "SceneHierarchyPanel.h"


#include "EntityPropertiesPanel.h"

#include<imgui.h>

namespace Resug
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> scene)
	{
		SetContext(scene);
	}
	void SceneHierarchyPanel::SetContext(const Ref<Scene> scene)
	{
		m_SelectEntity = {};
		m_Scene = scene;
	}
	void SceneHierarchyPanel::OnImGuiLayer()
	{
		ImGui::Begin("Scene Hierarchy");

		
		for (auto& e : m_Scene->GetRegistry().storage<entt::entity>()) 
		{
			// TODO : 封装下面的
			if (!m_Scene->GetRegistry().valid(e)) continue;
			Entity entity{ e, m_Scene.get()};

			DrawEntityToPanel(entity);
		}



		auto flag = ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems;
		if (ImGui::BeginPopupContextWindow(nullptr, flag))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Scene->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}



		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectEntity = {};
		}

		ImGui::End();


	}
	void SceneHierarchyPanel::DrawEntityToPanel(Entity entity)
	{
		bool entityShouldDelete = false;
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectEntity = entity;
		}

		std::string popupId = "EntityContextMenu_" + std::to_string((uint32_t)entity);
		if (ImGui::BeginPopupContextItem(popupId.c_str()))
		{
			ImGui::Text("%s", tag.c_str());
			if (ImGui::MenuItem("Destroy"))
				entityShouldDelete = true;
			ImGui::EndPopup();
		}
		//ImGui::PopID();

		if (opened)
		{
			ImGui::TreeNodeEx((void*)8889989, flags, tag.c_str());
			ImGui::TreePop();
		}

		if (entityShouldDelete)
		{
			if (m_SelectEntity == entity)
			{
				m_SelectEntity = {};
			}

			//TODO: check the entity delete completely .
			m_Scene->DestroyEntity(entity);


		}
	}
}


