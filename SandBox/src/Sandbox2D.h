#pragma once

#include"Resug.h"



class Sandbox2D : public Resug::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Resug::Timestep& ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Resug::Event& event) override;
private:
	Resug::OrthographicCameraController m_CameraController;

	Resug::Ref<Resug::ShaderLibrary> m_ShaderLibary;

	Resug::Ref<Resug::VertexArray> m_VertexArray;
	Resug::Ref<Resug::VertexArray> m_SquareVA;

	Resug::Ref<Resug::Texture2D> m_Texture;

	glm::vec4 m_SquareColor = { 0.2f,0.3f,0.0f,1.0f };


};

