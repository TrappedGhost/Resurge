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
		EditerCamera(glm::dmat4 projection);
		virtual ~EditerCamera() = default;

		void OnUpdate();


		CameraType GetCameraType() { return m_CameraType; }
		void SetCameraType(CameraType type) { m_CameraType = type; }

		double GetCameraSize() { return m_CameraSize; }
		double GetCameraNear() { return m_CameraNear; }
		double GetCameraFar() { return m_CameraFar; }
		glm::dvec3 GetCameraPosition() { return m_CameraPosition; }
		glm::dmat4 GetCameraTransform() { return m_CameraTransform; }

		void SetCameraSize(double size) { m_CameraSize = size; RecalculateProjection(); }
		void SetCameraFar(double size) { m_CameraFar = size; RecalculateProjection(); }
		void SetCameraNear(double size) { m_CameraNear = size; RecalculateProjection(); }

		void SetOrth(double size, double nearC, double farC);

		void SetViewportSize(uint32_t width, uint32_t height);
	private:
		void RecalculateProjection();
		void RecalculateTransform();
	private:

		glm::dmat4 m_CameraTransform = glm::dmat4(1.0f);

		glm::dvec3 m_CameraPosition = glm::dvec3(0.0f, 0.0f, 0.1f);
		glm::dvec2 m_PreMousePosition = glm::dvec2( 0.0f, 0.0f);


		CameraType m_CameraType = CameraType::Orthographic;

		double m_CameraSize = 10.0f;
		double m_CameraFar = 1.0f, m_CameraNear = -1.0f;

		double m_AspectRatio = 0.0f;



	};
}
