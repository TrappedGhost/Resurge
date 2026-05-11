#pragma once

#include<glm/glm.hpp>
namespace Resug
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(glm::dmat4 projection)
			:m_Projection(projection)
		{

		}

		virtual ~Camera() = default;

		glm::dmat4 GetProjection() { return m_Projection; }
	protected:
		glm::dmat4 m_Projection;
	};

}