#include "rgpch.h"
#include "EditerCamera.h"


#include"Resug/Core/Input.h"

#include<glm/gtc/matrix_transform.hpp>
namespace Resug
{
	EditerCamera::EditerCamera()
	{
		RecalculateProjection();
		RecalculateTransform();
	}
	EditerCamera::EditerCamera(glm::mat4 projection)
	{
		RecalculateProjection();
		RecalculateTransform();
	}
	void EditerCamera::OnUpdate()
	{
		if (Input::IsMouseButtonPressed(RG_MOUSE_BUTTON_MIDDLE))
		{

			glm::vec2 currentMousePosition = glm::vec2(Input::GetMousePosition().first, Input::GetMousePosition().second);
			glm::vec2 Displacement = (currentMousePosition - m_PreMousePosition)/50.0f;
			m_CameraPosition += glm::vec3(Displacement.x, -Displacement.y, 0.0f);
			
			RecalculateTransform();

			m_PreMousePosition = currentMousePosition;
			
			
		}
		else
		{
			m_PreMousePosition = glm::vec2(Input::GetMousePosition().first, Input::GetMousePosition().second);
		}
	}
	void EditerCamera::SetOrth(float size, float nearC, float farC)
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
		float orthoLefft = -m_CameraSize * m_AspectRatio * 0.5f;
		float orthoRight = m_CameraSize * m_AspectRatio * 0.5f;
		float orthoBottom = -m_CameraSize * 0.5f;
		float orthoTop = m_CameraSize * 0.5f;

		m_Projection = glm::ortho(orthoLefft, orthoRight, orthoBottom, orthoTop, m_CameraNear, m_CameraFar);

	}
	void EditerCamera::RecalculateTransform()
	{
		m_CameraTransform = glm::translate(glm::mat4(1.0f), m_CameraPosition);
	}
}