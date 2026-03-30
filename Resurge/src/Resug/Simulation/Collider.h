#pragma once



namespace Resug
{
	enum class ColliderType
	{
		None = 0,
		Box2D = 1,
		Mesh2D = 2,
		Circle = 3
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
