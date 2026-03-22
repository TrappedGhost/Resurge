#pragma once



namespace Resug
{
	enum class ColliderType
	{
		Box2D = 0,
		Mesh2D = 1,
		Circle = 2
	};

	class Collider
	{

	public:
		ColliderType m_Type;

		static std::vector<Collider*> Colliders;

	protected:
		float Ground = -3.0f;
	};

	
}
