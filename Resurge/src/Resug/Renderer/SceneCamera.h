#pragma once

#include"Camera.h"

namespace Resug
{
	class SceneCamera : public Camera
	{

	public:
		enum class CameraType
		{
			Projective = 0,
			Orthographic = 1
		};

		SceneCamera();
		SceneCamera(glm::mat4 projection);
		virtual ~SceneCamera() = default;
		
		CameraType GetCameraType() { return m_CameraType; }
		void SetCameraType(CameraType type) { m_CameraType = type; }

		float GetCameraSize() { return m_CameraSize; }
		float GetCameraNear() { return m_CameraNear; }
		float GetCameraFar() { return m_CameraFar; }

		void SetCameraSize(float size) { m_CameraSize = size; RecalculateProjection();}
		void SetCameraFar (float size) { m_CameraFar  = size; RecalculateProjection();}
		void SetCameraNear(float size) { m_CameraNear = size; RecalculateProjection();}

		void SetOrth(float size, float nearC, float farC);

		void SetViewportSize(uint32_t width, uint32_t height);
	private:
		void RecalculateProjection();
	private:

		CameraType m_CameraType = CameraType::Orthographic;

		float m_CameraSize = 10.0f;
		float m_CameraFar = 1.0f, m_CameraNear = -1.0f;

		float m_AspectRatio = 0.0f;
	};
}
