#include "rgpch.h"
#include "OrthographicCameraController.h"

#include"Resug/Core/KeyCode.h"
#include"Platform/Windows/WindowsInput.h"

namespace Resug
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool cameraRotationB)
		:m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_CameraRotationB(cameraRotationB)
	{
		m_AspectRatio = aspectRatio;
		m_CameraPosition = glm::vec3(0.0f);
	}
	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (Resug::Input::IsKeyPressed(RG_KEY_DOWN))
			m_CameraPosition.y += m_CameraPositionSpeed * ts;
		if (Resug::Input::IsKeyPressed(RG_KEY_UP))
			m_CameraPosition.y -= m_CameraPositionSpeed * ts;
		if (Resug::Input::IsKeyPressed(RG_KEY_LEFT))
			m_CameraPosition.x += m_CameraPositionSpeed * ts;
		if (Resug::Input::IsKeyPressed(RG_KEY_RIGHT))
			m_CameraPosition.x -= m_CameraPositionSpeed * ts;
	
		if (m_CameraRotationB)
		{
			if (Resug::Input::IsKeyPressed(RG_KEY_A))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			if (Resug::Input::IsKeyPressed(RG_KEY_D))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			m_Camera.SetRotation(m_CameraRotation);
		}
 
		m_Camera.SetPosition(m_CameraPosition);
	}
	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrollEvent>(RG_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(RG_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}
	bool OrthographicCameraController::OnMouseScrolled(MouseScrollEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		
		RG_CORE_WARN("{0},{1}", m_AspectRatio, m_ZoomLevel);
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}