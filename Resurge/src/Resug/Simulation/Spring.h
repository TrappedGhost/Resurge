#pragma once

namespace Resug
{
	class Spring
	{
	public:
		Spring(int a, int b, float restLength, float stiffness = 500.0f, float damping = 2.0f)
			: PointA(a), PointB(b), RestLength(restLength)
			, Stiffness(stiffness), Damping(damping)
		{
		}


		int PointA;
		int PointB;
		float RestLength;
		float Stiffness;
		float Damping;

	};
}
