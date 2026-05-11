#include "rgpch.h"
#include "CollisionDetection.h"
#include <cmath>

namespace Resug
{
	const double eps = 1e-6;



	double Dcmp(double x)
	{
		if (std::fabs(x) < eps)return 0;
		else return x;
	}

	glm::dvec3 Dcmp(glm::dvec3 x)
	{
		if (std::fabs(x.x) < eps && std::fabs(x.y) < eps && std::fabs(x.z) < eps)return glm::dvec3(0.0f);
		else return x;
	}

	glm::dvec3 Cross(glm::dvec3 a, glm::dvec3 b)
	{
		return glm::dvec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}

	double Dot(glm::dvec3 a, glm::dvec3 b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	bool PointOnSegment(glm::dvec3 point, glm::dvec3 seg1, glm::dvec3 seg2)
	{
		return (
			Dcmp(Cross(seg1 - point, seg2 - point)) == glm::dvec3(0.0f)
			&& (point.x - seg1.x) * (point.x - seg2.x) < 0
			&& (point.y - seg1.y) * (point.y - seg2.y) < 0
			);
	}

	bool GJK(glm::dvec3* polygon1, glm::dvec3* polygon2, uint32_t size1, uint32_t size2)
	{
		//TODO
		glm::dvec3 supportDirection = glm::dvec3(1.0f, 0.0f, 0.0f);

		uint32_t polygon1Index = 0;
		uint32_t polygon2Index = 0;

		double dot = Dot(polygon1[0], supportDirection);
		for (int i = 1; i < size1; i++)
		{
			double d = Dot(polygon1[i], supportDirection);
			if (dot < d)
			{
				dot = d;
				polygon1Index = i;
			}
		}

		dot = Dot(polygon2[0], supportDirection);
		for (int i = 1; i < size1; i++)
		{
			double d = Dot(polygon2[i], supportDirection);
			if (dot < d)
			{
				dot = d;
				polygon2Index = i;
			}
		}

		return false;
	}

	glm::dvec3 SAT(glm::dvec3* polygon1, glm::dvec3* polygon2, uint32_t size1, uint32_t size2)
	{
		return glm::dvec3(0.0f);
	}

	bool PointInPolygon(glm::dvec3 point, glm::dvec3* polygon, uint32_t size)
	{
		bool result = false;
		glm::dvec3 polygonPoint1, polygonPoint2;
		for (int i = 0, j = size - 1; i < size; j = i++)
		{
			polygonPoint1 = polygon[i];
			polygonPoint2 = polygon[j];
			//std::cout << polygonPoint1 << " " << polygonPoint2 << "\n";
			if (PointOnSegment(point, polygonPoint1, polygonPoint2))return true;

			if (Dcmp(polygonPoint1.y - polygonPoint2.y) == 0.0f)
			{

				result = !result;
				//std::cout << "PointInPolygon" << result << "\n";
				continue;
			}

			if ((polygonPoint1.y - point.y) * (polygonPoint2.y - point.y) < 0 &&
				point.x + Dcmp(polygonPoint1.y - point.y) * Dcmp(polygonPoint1.x - polygonPoint2.x) / Dcmp(polygonPoint1.y - polygonPoint2.y) - polygonPoint1.x < 0)
			{
				result = !result;
			}

		}
		//std::cout <<"PointInPolygon" << result << "\n";
		return result;
	}

	bool RayIntersectSegment(glm::dvec3 rayOrigin, glm::dvec3 rayDir,
							glm::dvec3 segStart, glm::dvec3 segEnd)
	{
		glm::dvec3 segDir = segEnd - segStart;
		glm::dvec3 s = rayOrigin - segStart;

		double denominator = glm::cross(segDir, rayDir).z;

		if (Dcmp(denominator) == 0.0f) {
			return false;
		}

		double u = glm::cross(s, rayDir).z / denominator;
		double t = glm::cross(s, segDir).z / denominator;

		return (u >= 0.0f && u <= 1.0f && t >= 0.0f);
	}

	glm::dvec3 CalculateVectorProjection(glm::dvec3 vector, glm::dvec3 dir)
	{
		double dot = glm::dot(vector, dir);
		double len2 = glm::dot(dir, dir);

		if (Dcmp(len2) == 0.0f) 
		{
			return glm::dvec3(0.0f);
		}

		return (dot / len2) * dir;
	}

	glm::dvec3 GetResultDisplacement(glm::dvec3 point, glm::dvec3 displacement,glm::dvec3* polygon, uint32_t size)
	{
		glm::dvec3 result = glm::dvec3(0.0f);
		glm::dvec3 polygonPoint1, polygonPoint2;
		for (int i = 0, j = size - 1; i < size; j = i++)
		{
			polygonPoint1 = polygon[i];
			polygonPoint2 = polygon[j];

			if (RayIntersectSegment(point, -displacement, polygonPoint1, polygonPoint2))
			{
				result = CalculateVectorProjection(displacement, polygonPoint1 - polygonPoint2);
				return result;
			}
			

		}
		return result;

	}
}