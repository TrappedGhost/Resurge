#pragma once
#include<glm/glm.hpp>
#include<cmath>

namespace Resug
{
	extern const double eps;

	double Dcmp(double x);
	float Dcmp(float x);
	glm::vec3 Cross(glm::vec3 a, glm::vec3 b);
	float Dot(glm::vec3 a, glm::vec3 b);
	bool PointOnSegment(glm::vec3 point, glm::vec3 seg1, glm::vec3 seg2);
	bool GJK(glm::vec3* polygon1, glm::vec3* polygon2, uint32_t size1, uint32_t size2);
	glm::vec3 SAT(glm::vec3* polygon1, glm::vec3* polygon2, uint32_t size1, uint32_t size2);
	bool PointInPolygon(glm::vec3 point, glm::vec3* polygon, uint32_t size);

}