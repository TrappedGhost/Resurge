#include "rgpch.h"
#include "SceneCamera.h"

#include<glm/gtc/matrix_transform.hpp>

Resug::SceneCamera::SceneCamera()
{
	RecalculateProjection();
}

Resug::SceneCamera::SceneCamera(glm::mat4 projection)
{
	//TODO
	RecalculateProjection();
}

void Resug::SceneCamera::SetOrth(float size, float nearC, float farC)
{
	m_CameraSize = size;
	m_CameraNear = nearC;
	m_CameraFar = farC;

	RecalculateProjection();
}

void Resug::SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
{
	m_AspectRatio = (float)width / (float)height;
	std::cout << " Resug::SceneCamera::SetViewportSize" << m_AspectRatio << "\n";
	RecalculateProjection();
}

void Resug::SceneCamera::RecalculateProjection()
{
	float orthoLefft = -m_CameraSize * m_AspectRatio * 0.5f;
	float orthoRight = m_CameraSize * m_AspectRatio * 0.5f;
	float orthoBottom = -m_CameraSize * 0.5f;
	float orthoTop = m_CameraSize * 0.5f;

	m_Projection = glm::ortho(orthoLefft, orthoRight, orthoBottom, orthoTop, m_CameraNear, m_CameraFar);
}
