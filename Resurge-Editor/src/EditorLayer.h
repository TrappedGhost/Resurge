#pragma once

#include"Resug.h"

#include"Panel/SceneHierarchyPanel.h"
#include"Panel/EntityPropertiesPanel.h"

class EditorLayer : public Resug::Layer
{
public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Resug::Timestep& ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Resug::Event& event) override;
	bool OnKeyPressed(Resug::KeyPressedEvent& event);
private:
	Resug::OrthographicCameraController m_CameraController;

	Resug::Ref<Resug::ShaderLibrary> m_ShaderLibary;

	Resug::Ref<Resug::VertexArray> m_VertexArray;
	Resug::Ref<Resug::VertexArray> m_SquareVA;

	Resug::Ref<Resug::Texture2D> m_Texture;
	Resug::Ref<Resug::SubTexture2D> m_TextureInterface;
	Resug::Ref<Resug::FrameBuffer> m_FrameBuffer;

	Resug::Ref<Resug::Scene> m_ActiveScene;
	//Resug::Entity m_SquareEntity;
	//Resug::Entity m_CamearEntity;
	//Resug::Entity m_SecondCameraEntity;

	bool m_PrimaryCamera = false;

	glm::vec2 m_ViewportSize = glm::vec2(1280.0f,720.0f);
	glm::vec2 m_FrameTexSize = glm::vec2(1280.0f,720.0f);

	bool m_ViewportFoucses = false, m_ViewportHovered = false;

	glm::vec4 m_SquareColor = { 0.2f,0.3f,0.0f,1.0f };

	Resug::SceneHierarchyPanel m_SceneHierarchyPanel;
	Resug::EntityPropertiesPanel m_EntityPropertiesPanel;
	Resug::EditerCamera m_EditerCamera;//TODO：增加缩放，限制操作在视口内。

	uint32_t m_GizomType = -1;

	Resug::Serializer m_Serializer;

	enum class EditerStatus
	{
		Edit = 0,
		Play = 1,
		Stop = 2
	};

	EditerStatus m_EditerStatus = EditerStatus::Edit;

};

