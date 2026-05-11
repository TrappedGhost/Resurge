#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Resug {

    struct FEMNode
    {
        glm::dvec2 Position;
        glm::dvec2 PrevPosition;
        glm::dvec2 Velocity;
        glm::dvec2 Force;
        double Mass;
        bool Fixed;

        FEMNode(const glm::dvec2& pos, double mass = 1.0f, bool fixed = false)
            : Position(pos), PrevPosition(pos), Velocity(0.0f), Force(0.0f)
            , Mass(mass), Fixed(fixed)
        {
        }
    };

    struct TriangleElement
    {
        int NodeIndices[3];
        double YoungsModulus; 
        double PoissonRatio;  
        double Thickness;     

        glm::dmat2 InverseReferenceMatrix;
        double ReferenceArea;

        TriangleElement(int n0, int n1, int n2, double E = 1000.0f, double nu = 0.3f, double thickness = 1.0f)
            : YoungsModulus(E), PoissonRatio(nu), Thickness(thickness)
        {
            NodeIndices[0] = n0;
            NodeIndices[1] = n1;
            NodeIndices[2] = n2;
        }
    };

    class FEMSystem2D
    {
    public:
        FEMSystem2D() = default;

        void AddNode(const glm::dvec2& position, double mass = 1.0f, bool fixed = false)
        {
            m_Nodes.emplace_back(position, mass, fixed);
        }

        void AddTriangle(int n0, int n1, int n2, double stiffness = 100000.0f, double poisson = 0.3f)
        {
            TriangleElement elem(n0, n1, n2, stiffness, poisson);

            // 计算参考构型下的矩阵 D_m 和面积
            glm::dvec2 X0 = m_Nodes[n0].Position;
            glm::dvec2 X1 = m_Nodes[n1].Position;
            glm::dvec2 X2 = m_Nodes[n2].Position;

            glm::dmat2 Dm(X1 - X0, X2 - X0); // 列向量构造矩阵

            elem.ReferenceArea = 0.5 * glm::determinant(Dm);

            // 防止除零
            if (std::abs(elem.ReferenceArea) > 1e-6) {
                elem.InverseReferenceMatrix = glm::inverse(Dm);
            }
            else {
                elem.InverseReferenceMatrix = glm::dmat2(0.0);
            }

            m_Elements.push_back(elem);
        }

        void SetNodePosition(int i, const glm::dvec2& pos) { m_Nodes[i].Position = pos; }
        glm::dvec2 GetNodePosition(int i) { return m_Nodes[i].Position; }
        glm::dvec2 GetNodeVelocity(int i) { return m_Nodes[i].Velocity; }

        bool GetInitialize() { return m_IsInitialized; }
        void SetInitialize(bool bo) { m_IsInitialized = bo; }
        operator bool() { return m_IsInitialized; }

        void OnAttach();
        void OnUpdate(double dt); // 这里简化直接用 double dt，也可以换成 Timestep

        void ComputeForces();
        void Integrate(double dt);
        void IntegrateImplicitNewton(double dt);


        bool m_IsInitialized = false;
        std::vector<FEMNode> m_Nodes;
        std::vector<TriangleElement> m_Elements;

        glm::dvec2 m_Gravity = glm::dvec2(0.0f, -9.8f);
        double m_Damping = 0.05f; // 简单的全局阻尼
    };

}