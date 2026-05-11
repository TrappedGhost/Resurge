#include "rgpch.h"
#include "EditerCamera.h"


#include"Resug/Core/Input.h"

#include <glm/gtc/matrix_transform.hpp>  


namespace Resug
{
	EditerCamera::EditerCamera()
	{
		RecalculateProjection();
		RecalculateTransform();
	}
	EditerCamera::EditerCamera(glm::dmat4 projection)
	{
		RecalculateProjection();
		RecalculateTransform();
	}
	void EditerCamera::OnUpdate()
	{
		if (Input::IsMouseButtonPressed(RG_MOUSE_BUTTON_MIDDLE))
		{

			glm::dvec2 currentMousePosition = glm::dvec2(Input::GetMousePosition().first, Input::GetMousePosition().second);
			glm::dvec2 Displacement = (currentMousePosition - m_PreMousePosition) / 50.0;
			m_CameraPosition += glm::dvec3(Displacement.x, -Displacement.y, 0.0);

			RecalculateTransform();

			m_PreMousePosition = currentMousePosition;
			
			
		}
		else
		{
			m_PreMousePosition = glm::dvec2(Input::GetMousePosition().first, Input::GetMousePosition().second);
		}
	}
	void EditerCamera::SetOrth(double size, double nearC, double farC)
	{
		m_CameraSize = size;
		m_CameraNear = nearC;
		m_CameraFar = farC;

		RecalculateProjection();
	}
	void EditerCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		std::cout << " Resug::SceneCamera::SetViewportSize" << m_AspectRatio << "\n";
		RecalculateProjection();
	}
	void EditerCamera::RecalculateProjection()
	{
		double orthoLefft = -m_CameraSize * m_AspectRatio * 0.5;
		double orthoRight = m_CameraSize * m_AspectRatio * 0.5;
		double orthoBottom = -m_CameraSize * 0.5;
		double orthoTop = m_CameraSize * 0.5;

		m_Projection = glm::ortho(orthoLefft, orthoRight, orthoBottom, orthoTop, m_CameraNear, m_CameraFar);

	}
	void EditerCamera::RecalculateTransform()
	{
		m_CameraTransform = glm::translate(glm::dmat4(1.0), m_CameraPosition);
	}
}