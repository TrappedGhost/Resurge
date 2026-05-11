#pragma once
#include<glm/glm.hpp>
#include<cmath>

#include"MeshCollider2D.h"
#include"BoxCollider2D.h"

namespace Resug
{
	extern const double eps;

	double Dcmp(double x);
	double Dcmp(double x);
	glm::dvec3 Cross(glm::dvec3 a, glm::dvec3 b);
	double Dot(glm::dvec3 a, glm::dvec3 b);
	bool PointOnSegment(glm::dvec3 point, glm::dvec3 seg1, glm::dvec3 seg2);
	bool GJK(glm::dvec3* polygon1, glm::dvec3* polygon2, uint32_t size1, uint32_t size2);
	glm::dvec3 SAT(glm::dvec3* polygon1, glm::dvec3* polygon2, uint32_t size1, uint32_t size2);
	bool PointInPolygon(glm::dvec3 point, glm::dvec3* polygon, uint32_t size);
	glm::dvec3 GetResultDisplacement(glm::dvec3 point, glm::dvec3 displacement, glm::dvec3* polygon, uint32_t size);
}