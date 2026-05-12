#pragma once

#include<glm/glm.hpp>

namespace Resug
{

	struct ConvexPolygon2D
	{
		std::vector<glm::dvec3> vertiesPosition;
		double restitution;          
		double friction;            

		ConvexPolygon2D()
			: restitution(0.6), friction(0.2)
		{
		}

		ConvexPolygon2D(const std::vector<glm::dvec3>& verts)
			: vertiesPosition(verts), restitution(0.6), friction(0.2)
		{
		}
	};

	class Geometry2D
	{
	public:
		static std::vector<Ref<ConvexPolygon2D>> Polygons;
		
		static double PointToSegmentDistance(const glm::dvec3& point, const glm::dvec3& s1, const glm::dvec3& s2);
		
		static double PointToPolygonDistance(const glm::dvec3& point, const std::vector<glm::dvec3>& polygon);

		static glm::dvec3 PolygonNormal(const ConvexPolygon2D& polygon);

		static glm::dvec3 ClosestPointOnSegment(const glm::dvec3& point, const glm::dvec3& s1, const glm::dvec3& s2);

		static bool IsPointInsideConvexPolygon(const glm::dvec3& point, const ConvexPolygon2D& polygon);
	};

}