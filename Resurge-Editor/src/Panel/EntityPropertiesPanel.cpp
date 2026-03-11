#include "EntityPropertiesPanel.h"

#include"imgui.h"

#include"Resug/Scene/Components.h"

#include<iostream>
#include"glm/gtc/type_ptr.hpp"

namespace Resug
{
    EntityPropertiesPanel::EntityPropertiesPanel(Entity& entity)
        :m_Entity(entity)
    {
    }

    void EntityPropertiesPanel::OnImGuiLayer(Entity entity)
    {
        m_Entity = entity;
        
        ImGui::Begin("Entity Properties");
        if (!m_Entity)
        {
            ImGui::End();
            return;
        }
        DrawComponentToPanel(entity);

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");
        if (ImGui::BeginPopup("AddComponent"))
        {
            if (ImGui::MenuItem("Camera"))
            {
                entity.AddComponent<CameraComponent>(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Sprite Renderer"))
            {
                entity.AddComponent<SpriteRendererComponent>();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }


        ImGui::End();
    }

    void EntityPropertiesPanel::DrawComponentToPanel(Entity entity)
    {

        if (m_Entity.HasComponent<TagComponent>())
        {
            auto& tag = m_Entity.GetComponent<TagComponent>().Tag;
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }

        ImGuiTreeNodeFlags treeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;


        if (m_Entity.HasComponent<TransformComponent>())
        {
            bool componentShouldDelete = false;

            bool treeNode = (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeFlag, "Transform"));
            ImGui::SameLine();
            if (ImGui::Button("Component Setting"))
                ImGui::OpenPopup("ComponentSetting");
            if (ImGui::BeginPopup("ComponentSetting"))
            {
                if (ImGui::MenuItem("Delete Component"))
                    componentShouldDelete = true;
                ImGui::EndPopup();
            }

            if(treeNode)
            {
                auto& transform = m_Entity.GetComponent<TransformComponent>();
                if (ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.1f))
                    transform.RecalculateTransform();
                if (ImGui::DragFloat3("Rotation", glm::value_ptr(transform.Rotation), 0.1f))
                    transform.RecalculateTransform();
                if (ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.1f))
                    transform.RecalculateTransform();

                    ImGui::TreePop();
            }

            if (componentShouldDelete)
                m_Entity.RemoveComponent<TransformComponent>();

        }


        if (m_Entity.HasComponent<CameraComponent>())
        {
            bool componentShouldDelete = false;

            bool treeNode = (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeFlag, "Camera"));
            ImGui::SameLine();
            if (ImGui::Button("Component Setting"))
                ImGui::OpenPopup("ComponentSetting");
            if (ImGui::BeginPopup("ComponentSetting"))
            {
                if (ImGui::MenuItem("Delete Component"))
                    componentShouldDelete = true;
                ImGui::EndPopup();
            }
            if (treeNode)
            {
                auto& cameraComponent = m_Entity.GetComponent<CameraComponent>();
                auto& camera = cameraComponent.camera;

                if (ImGui::Checkbox("Camera Primary", &cameraComponent.Primary));


                const char* cameraTypeString[] = { "Prejection", "Orth" };
                int cameraTypeIndex = (int)camera.GetCameraType();
                if (ImGui::BeginCombo("CameraType", cameraTypeString[cameraTypeIndex]))
                {
                    for (int i = 0; i < 2; i++)
                    {
                        bool isSelected = cameraTypeIndex == i;
                        if (ImGui::Selectable(cameraTypeString[i], isSelected))
                        {
                            cameraTypeIndex = i;
                            cameraComponent.camera.SetCameraType((SceneCamera::CameraType)i);
                        }

                        if (isSelected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }

                if (camera.GetCameraType() == SceneCamera::CameraType::Projective)
                {
                }
                if (camera.GetCameraType() == SceneCamera::CameraType::Orthographic)
                {
                    float orthCameraSize = camera.GetCameraSize();
                    if (ImGui::DragFloat("Size", &orthCameraSize))
                        camera.SetCameraSize(orthCameraSize);

                    float orthCameraNear = camera.GetCameraNear();
                    if (ImGui::DragFloat("Near", &orthCameraNear))
                        camera.SetCameraNear(orthCameraNear);

                    float orthCameraFar = camera.GetCameraFar();
                    if (ImGui::DragFloat("Far", &orthCameraFar))
                        camera.SetCameraFar(orthCameraFar);
                }
                ImGui::TreePop();
            }

            if (componentShouldDelete)
                m_Entity.RemoveComponent<SpriteRendererComponent>();
        }

        if (m_Entity.HasComponent<SpriteRendererComponent>())
        {
            bool componentShouldDelete = false;

            bool treeNode = (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeFlag, "SpriteRenderer"));
            ImGui::SameLine();
            if (ImGui::Button("Component Setting"))
                ImGui::OpenPopup("ComponentSetting");
            if (ImGui::BeginPopup("ComponentSetting"))
            {
                if (ImGui::MenuItem("Delete Component"))
                    componentShouldDelete = true;
                ImGui::EndPopup();
            }
            if (treeNode)
            {
                auto& color = m_Entity.GetComponent<SpriteRendererComponent>().Color;
                ImGui::ColorEdit4("Color", glm::value_ptr(color));
                ImGui::TreePop();
            }

            if (componentShouldDelete)
                m_Entity.RemoveComponent<SpriteRendererComponent>();
        }
    }
}