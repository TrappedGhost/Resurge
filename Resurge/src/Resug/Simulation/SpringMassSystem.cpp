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
            point.Force += m_Gravity * point.Mass;
            
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

                float forceMagnitude = spring.Stiffness * (currentLength - spring.RestLength)*100;
                
                glm::vec3 springForce = dir * forceMagnitude;
                //std::cout << dir << "\n ";
                a.Force += springForce;
                b.Force -= springForce;
            }
        }

    }

    void Resug::SpringMassSystem::IntegrateVerlet(float dt)
    {
        dt /= 10.0f;

        for (auto& point : m_Points)
        {

            glm::vec3 acceleration = point.Force / point.Mass;

            point.Velocity += acceleration* dt;

        }
    }
}