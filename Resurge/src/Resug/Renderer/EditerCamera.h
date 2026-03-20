#pragma once

#include"Camera.h"

#include<glm/glm.hpp>

namespace Resug
{
	class EditerCamera:public Camera
	{
	public:
		enum class CameraType
		{
			Projective = 0,
			Orthographic = 1
		};
		
		EditerCamera();
		EditerCamera(glm::mat4 projection);
		virtual ~EditerCamera() = default;

		void OnUpdate();


		CameraType GetCameraType() { return m_CameraType; }
		void SetCameraType(CameraType type) { m_CameraType = type; }

		float GetCameraSize() { return m_CameraSize; }
		float GetCameraNear() { return m_CameraNear; }
		float GetCameraFar() { return m_CameraFar; }
		glm::vec3 GetCameraPosition() { return m_CameraPosition; }
		glm::mat4 GetCameraTransform() { return m_CameraTransform; }

		void SetCameraSize(float size) { m_CameraSize = size; RecalculateProjection(); }
		void SetCameraFar(float size) { m_CameraFar = size; RecalculateProjection(); }
		void SetCameraNear(float size) { m_CameraNear = size; RecalculateProjection(); }

		void SetOrth(float size, float nearC, float farC);

		void SetViewportSize(uint32_t width, uint32_t height);
	private:
		void RecalculateProjection();
		void RecalculateTransform();
	private:

		glm::mat4 m_CameraTransform = glm::mat4(1.0f);

		glm::vec3 m_CameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec2 m_PreMousePosition = glm::vec2( 0.0f, 0.0f);


		CameraType m_CameraType = CameraType::Orthographic;

		float m_CameraSize = 10.0f;
		float m_CameraFar = 1.0f, m_CameraNear = -1.0f;

		float m_AspectRatio = 0.0f;



	};
}
