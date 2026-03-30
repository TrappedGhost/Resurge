#pragma once

namespace Resug {

    struct MassPoint
    {
        glm::vec3 Position;
        glm::vec3 PrevPosition;
        glm::vec3 Velocity;
        glm::vec3 Force;
        float Mass;
        bool Fixed;

        MassPoint(const glm::vec3& pos, float mass = 1.0f, bool fixed = false)
            : Position(pos), PrevPosition(pos), Velocity(0.0f), Force(0.0f)
            , Mass(mass), Fixed(fixed)
        {
        }
    };

    struct Spring
    {
        int PointA;
        int PointB;
        float RestLength;
        float Stiffness;
        float Damping;

        Spring(int a, int b, float restLength, float stiffness = 10000.0f, float damping = 1.0f)
            : PointA(a), PointB(b), RestLength(restLength)
            , Stiffness(stiffness), Damping(damping)
        {
        }
    };



    class SpringMassSystem
    {
    public:
        SpringMassSystem() = default;

        void AddPoint(const glm::vec3& position, float mass = 1.0f, bool fixed = false)
        {
            m_Points.emplace_back(position, mass, fixed);
        }
        void AddPoint(const glm::vec4& position, float mass = 1.0f, bool fixed = false)
        {
            m_Points.emplace_back(glm::vec3(position), mass, fixed);
        }

        void AddSpring(int a, int b, float stiffness = 100.0f, float damping = 1.0f)
        {
            float restLength = glm::distance(m_Points[a].Position, m_Points[b].Position);
            m_Springs.emplace_back(a, b, restLength, stiffness, damping);
        }

        void SetPointPosition(int i, glm::vec3 position)
        {
            m_Points[i].Position = position;
        }
        void SetPointPosition(int i, glm::vec4 position)
        {
            m_Points[i].Position = position;
        }
        glm::vec4 GetPointPosition(int i){return glm::vec4(m_Points[i].Position, 1.0f);}
        glm::vec4 GetPointVelocity(int i){return glm::vec4(m_Points[i].Velocity, 1.0f);}
        glm::vec4 GetPointForcef4(int i){ return glm::vec4(m_Points[i].Force, 1.0f);        }
        glm::vec3 GetPointForcef3(int i){ return glm::vec3(m_Points[i].Force);        }

        bool GetIntialize() { return isIntialize; }
        void SetIntialize(bool bo) { isIntialize = bo; }

        void OnAttach();

        void OnUpdate(Timestep& ts);

        operator bool() { return isIntialize; }
    private:
        void ComputeForces();
        
        void IntegrateVerlet(float dt);
    private:
        bool isIntialize = false;

        std::vector<MassPoint> m_Points;
        std::vector<Spring> m_Springs;
        glm::vec3 m_Gravity = glm::vec3(0.0f, -9.8f, 0.0f);
        float m_GlobalDamping = 0.1f;
    };
}

