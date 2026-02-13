#pragma once


#include"Resug/Core/Core.h"
#include"Resug/Renderer/OrthographicCamera.h"
#include"Resug/Core/Timestep.h"

#include"Resug/Event/ApplicationEvent.h"
#include"Resug/Event/MouseEvent.h"

namespace Resug
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool cameraRotationB = false);
		
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera GetCamera() { return m_Camera; }
		const OrthographicCamera  GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrollEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		
		glm::vec3 m_CameraPosition;
		float m_CameraPositionSpeed = 1.0f;

		bool m_CameraRotationB;
		float m_CameraRotation = 0.0F;
		float m_CameraRotationSpeed = 2.1f;

		OrthographicCamera m_Camera;

	};
}
