// SpringMassSystem.cpp
#include"rgpch.h"

#include "SpringMassSystem.h"
#include <iostream>

namespace Resug
{
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
}