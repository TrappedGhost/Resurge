#pragma once


#include"CollisionDetection.h"

namespace Resug
{
	enum class ColliderType
	{
		Box2D = 0,
		Circle = 1
	};

	class Collider
	{

	public:
		ColliderType m_Type;
	protected:
		float Ground = -3.0f;
	};

	class BoxCollider2D:public Collider
	{
	public:
		BoxCollider2D();

		glm::vec3 OnUpdate(float ts, glm::vec3 velocity);



		static std::vector<BoxCollider2D*> BoxCollider2Ds;

		glm::vec3 m_VertexPosition[4];
		glm::vec3 m_VertexVelocity[4];
		glm::vec3 m_VertexDisplacement[4];
		uint32_t m_VertexSize = 4;

		ColliderType m_Type = ColliderType::Box2D;
	
		bool m_OnGround = false;
	};
}
