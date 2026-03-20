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

	float Dcmp(float x)
	{
		if (std::fabs(x) < eps)return 0;
		else return x;
	}

	glm::vec3 Dcmp(glm::vec3 x)
	{
		if (std::fabs(x.x) < eps && std::fabs(x.y) < eps && std::fabs(x.z) < eps)return glm::vec3(0.0f);
		else return x;
	}

	glm::vec3 Cross(glm::vec3 a, glm::vec3 b)
	{
		return glm::vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}

	float Dot(glm::vec3 a, glm::vec3 b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	bool PointOnSegment(glm::vec3 point, glm::vec3 seg1, glm::vec3 seg2)
	{
		return (
			Dcmp(Cross(seg1 - point, seg2 - point)) == glm::vec3(0.0f)
			&& (point.x - seg1.x) * (point.x - seg2.x) < 0
			&& (point.y - seg1.y) * (point.y - seg2.y) < 0
			);
	}

	bool GJK(glm::vec3* polygon1, glm::vec3* polygon2, uint32_t size1, uint32_t size2)
	{
		//TODO
		glm::vec3 supportDirection = glm::vec3(1.0f, 0.0f, 0.0f);

		uint32_t polygon1Index = 0;
		uint32_t polygon2Index = 0;

		float dot = Dot(polygon1[0], supportDirection);
		for (int i = 1; i < size1; i++)
		{
			float d = Dot(polygon1[i], supportDirection);
			if (dot < d)
			{
				dot = d;
				polygon1Index = i;
			}
		}

		dot = Dot(polygon2[0], supportDirection);
		for (int i = 1; i < size1; i++)
		{
			float d = Dot(polygon2[i], supportDirection);
			if (dot < d)
			{
				dot = d;
				polygon2Index = i;
			}
		}

		return false;
	}

	glm::vec3 SAT(glm::vec3* polygon1, glm::vec3* polygon2, uint32_t size1, uint32_t size2)
	{
		return glm::vec3(0.0f);
	}

	bool PointInPolygon(glm::vec3 point, glm::vec3* polygon, uint32_t size)
	{
		bool result = false;
		glm::vec3 polygonPoint1, polygonPoint2;
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
}