// SpringMassSystem.cpp
#include"rgpch.h"

#include "SpringMassSystem.h"
#include <iostream>

namespace Resug
{
    void SpringMassSystem::OnUpdate(double dt)
    {
        //if (!isIntialize) return;

        ComputeForces();
        IntegrateSemiImplicitEuler(dt);
        HandleGroundCollision();

		for (auto& polygon : Geometry2D::Polygons)
        {
            HandleConvexPolygonCollision(*polygon);
			//std::cout << polygon->vertiesPosition[0] << " " << polygon->vertiesPosition[1] << " " << polygon->vertiesPosition[2] << " " << polygon->vertiesPosition[3] << "\n";
        }

    }
    void SpringMassSystem::ComputeForces()
    {
        // 重置所有力
        for (auto& point : m_Points)
        {
            point.Force = glm::dvec3(0.0f);
        }

        // 重力
        for (auto& point : m_Points)
        {
            point.Force += m_Gravity * point.Mass;
        }

        // 弹簧力（带阻尼）
        for (const auto& spring : m_Springs)
        {
            MassPoint& a = m_Points[spring.PointA];
            MassPoint& b = m_Points[spring.PointB];

            glm::dvec3 dir = b.Position - a.Position;
            double currentLength = glm::length(dir);

            if (currentLength > 1e-6f)
            {
                dir /= currentLength;

                // 弹簧力（弹性部分）
                double springForceMag = spring.Stiffness * (currentLength - spring.RestLength);

                // 相对速度在弹簧方向的分量
                glm::dvec3 relativeVel = b.Velocity - a.Velocity;
                double velAlongSpring = glm::dot(relativeVel, dir);

                // 阻尼力（耗散部分）
                double dampingForceMag = spring.Damping * velAlongSpring;

                // 总力
                double totalForceMag = springForceMag + dampingForceMag;
                glm::dvec3 force = dir * totalForceMag;

                a.Force += force;
                b.Force -= force;
            }
        }
        //std::cout << m_Points[1].Force << " ";
    }

    void SpringMassSystem::IntegrateSemiImplicitEuler(double dt)
    {
        // dt 是秒（外部传入已经是秒）
        if (dt > 0.033f) dt = 0.033f;  // 限制最大步长

        for (auto& point : m_Points)
        {
            if (point.Fixed) continue;

            // 计算加速度
            glm::dvec3 acceleration = point.Force / point.Mass;

            // 半隐式欧拉：先更新速度
            point.Velocity += acceleration * dt;

            // 限制最大速度（防止爆炸）
            double maxSpeed = 15.0;
            if (glm::length(point.Velocity) > maxSpeed)
            {
                point.Velocity = glm::normalize(point.Velocity) * maxSpeed;
            }

            // 再更新位置
            point.Position += point.Velocity * dt;

        }
            //std::cout << m_Points[5].Velocity << "\n";
    }

    void SpringMassSystem::HandleGroundCollision()
    {
        for (auto& point : m_Points)
        {
            if (point.Fixed) continue;

            // 检查是否穿透地面
            if (point.Position.y < m_GroundY)
            {
                // 修正位置到地面
                point.Position.y = m_GroundY;

                // 只有向下运动时才反弹
                if (point.Velocity.y < 0)
                {
                    // 反转垂直速度并乘以恢复系数
                    point.Velocity.y = -point.Velocity.y * m_Restitution;

                    // 地面摩擦力（可选）
                    point.Velocity.x *= 0.98f;
                    point.Velocity.z *= 0.98f;

                    // 如果速度太小，直接停止（避免微小抖动）
                    if (std::abs(point.Velocity.y) < 0.5f)
                    {
                        point.Velocity.y = 0;
                    }
                }
            }
        }
    }
    void SpringMassSystem::HandleConvexPolygonCollision(const ConvexPolygon2D& polygon)
    {
        for (auto& point : m_Points)
         {
             if (point.Fixed) continue;
             // 简单的点-多边形碰撞检测
             if (!Geometry2D::IsPointInsideConvexPolygon(point.Position, polygon))continue;
             glm::dvec3 closestPoint, penetration, dir, s1, s2;
			 uint32_t polygonSize = polygon.vertiesPosition.size();
             double minDistance = Geometry2D::PointToSegmentDistance(point.Position, polygon.vertiesPosition[0], polygon.vertiesPosition[1]);
			 for (uint32_t i = 1; i < polygonSize; i++)
             {
				 uint32_t j = (i + 1) % polygonSize;
				 double distance = Geometry2D::PointToSegmentDistance(point.Position, polygon.vertiesPosition[i], polygon.vertiesPosition[j]);
                 if (minDistance > distance)
                 {
                     minDistance = distance;
					 s1 = polygon.vertiesPosition[i];
					 s2 = polygon.vertiesPosition[j];
                 }
             }
			 closestPoint = Geometry2D::ClosestPointOnSegment(point.Position, s1, s2);
             penetration = point.Position - closestPoint;

             double depth = glm::length(penetration);
			 if (depth < 0.01f) continue; // 避免过度修正

			 dir = glm::normalize(penetration);

             dir = -dir;
             

             point.Position = closestPoint + dir * 0.001;

			 // 反弹
             // 计算法线方向的速度分量
             double velAlongNormal = glm::dot(point.Velocity, dir);

             if (velAlongNormal > 0) continue;

             double restitution = polygon.restitution;

             // 计算新的法线方向速度
             double newVelAlongNormal = -velAlongNormal * restitution;


             // 更新速度（切向分量保留，加上摩擦）
             glm::dvec3 tangentVelocity = point.Velocity - dir * velAlongNormal;

             // 应用摩擦
             double friction = polygon.friction;
             tangentVelocity *= (1.0 - friction);

             // 合成新速度
             point.Velocity = tangentVelocity + dir * newVelAlongNormal;

             // 防止微小抖动
             if (glm::length(point.Velocity) < 0.01)
             {
                 point.Velocity = glm::dvec3(0);
             }
		}
    }
}