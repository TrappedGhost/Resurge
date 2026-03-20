#include "rgpch.h"
#include "SpringMassSystem.h"

namespace Resug
{
    void Resug::SpringMassSystem::OnAttach()
    {
        for(int i = 0; i < m_Points.size(); i++)
        {
            for (int j = i + 1; j < m_Points.size(); j++)
            {
                AddSpring(i, j);
            }
        }
    }

    void Resug::SpringMassSystem::OnUpdate(Timestep& ts)
    {
        ComputeForces();

        IntegrateVerlet(ts);
    }

    void Resug::SpringMassSystem::ComputeForces()
    {
        for (auto& point : m_Points)
        {
            point.Force = glm::vec3(0.0f);
        }

        for (auto& point : m_Points)
        {
            if (!point.Fixed)
            {
                point.Force += m_Gravity * point.Mass;
            }
        }

        for (const auto& spring : m_Springs)
        {
            MassPoint& a = m_Points[spring.PointA];
            MassPoint& b = m_Points[spring.PointB];

            glm::vec3 dir = b.Position - a.Position;
            float currentLength = glm::length(dir);

            if (currentLength > 0.0f)
            {
                dir /= currentLength;  

                float forceMagnitude = spring.Stiffness * (currentLength - spring.RestLength);
                glm::vec3 springForce = dir * forceMagnitude;


                ///TODO: 添加阻尼力

                if (!a.Fixed)
                    a.Force += springForce ;
                if (!b.Fixed)
                    b.Force -= springForce ;
            }
        }

    }

    void Resug::SpringMassSystem::IntegrateVerlet(float dt)
    {
        dt /= 10.0f;
        float groundY = -3.0f;  
        float restitution = 1.0f;  

        for (auto& point : m_Points)
        {
            if (point.Fixed) continue;

            // Verlet积分
            glm::vec3 acceleration = point.Force / point.Mass;
            glm::vec3 newPosition = point.Position + (point.Position - point.PrevPosition) + acceleration * dt * dt;

            // 更新速度
            point.Velocity = (newPosition - point.Position) / dt;

            // 更新位置
            point.PrevPosition = point.Position;
            point.Position = newPosition;

            // 地面碰撞检测
            if (point.Position.y < groundY)
            {
                point.Position.y = groundY + (groundY - point.Position.y) * restitution;
                point.Velocity.y = -point.Velocity.y * restitution;
            }
        }
    }
}