#include "rgpch.h"
#include "Geometry2D.h"


namespace Resug
{
	std::vector<Ref<ConvexPolygon2D>> Geometry2D::Polygons;

	double Geometry2D::PointToSegmentDistance(const glm::dvec3& point, const glm::dvec3& s1, const glm::dvec3& s2)
	{
		glm::dvec3 segment = s2 - s1;
		double t = glm::dot(point - s1, segment) / glm::dot(segment, segment);
		t = glm::clamp(t, 0.0, 1.0);
		glm::dvec3 closestPoint = s1 + segment * t;
		return glm::distance(point, closestPoint);
	}

	double Geometry2D::PointToPolygonDistance(const glm::dvec3& point, const std::vector<glm::dvec3>& polygon)
	{
		if (polygon.size() < 3) return std::numeric_limits<double>::max();
		double minDistance = std::numeric_limits<double>::max();
		for (size_t i = 0; i < polygon.size(); i++)
		{
			size_t j = (i + 1) % polygon.size();
			double distance = PointToSegmentDistance(point, polygon[i], polygon[j]);
			minDistance = std::min(minDistance, distance);
		}
		return minDistance;
	}

	glm::dvec3 Geometry2D::PolygonNormal(const ConvexPolygon2D& polygon)
	{
		if (polygon.vertiesPosition.size() < 3) return glm::dvec3(0, 1, 0);
		// 使用前三个点计算法线
		glm::dvec3 edge1 = polygon.vertiesPosition[1] - polygon.vertiesPosition[0];
		glm::dvec3 edge2 = polygon.vertiesPosition[2] - polygon.vertiesPosition[0];
		glm::dvec3 normal = glm::normalize(glm::cross(edge1, edge2));
		return normal;
	}

	glm::dvec3 Geometry2D::ClosestPointOnSegment(const glm::dvec3& point, const glm::dvec3& s1, const glm::dvec3& s2)
	{
		glm::dvec3 segment = s2 - s1;
		double t = glm::dot(point - s1, segment) / glm::dot(segment, segment);
		t = glm::clamp(t, 0.0, 1.0);
		return s1 + segment * t;
	}

	bool Geometry2D::IsPointInsideConvexPolygon(const glm::dvec3& point, const ConvexPolygon2D& polygon)
	{
		if (polygon.vertiesPosition.size() < 3) return false;
		glm::dvec3 normal = PolygonNormal(polygon);
		// 对于凸多边形，检查点是否在所有边的同一侧
		for (size_t i = 0; i < polygon.vertiesPosition.size(); i++)
		{
			size_t j = (i + 1) % polygon.vertiesPosition.size();
			glm::dvec3 edge = polygon.vertiesPosition[j] - polygon.vertiesPosition[i];
			glm::dvec3 toPoint = point - polygon.vertiesPosition[i];
			glm::dvec3 cross = glm::cross(edge, toPoint);
			if (glm::dot(cross, normal) < 0)
				return false; // 点在边的外侧
		}
		return true; // 点在所有边的同一侧
	}
}
