#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Resug {

    // 2D 节点结构
    struct FEMNode
    {
        glm::vec2 Position;
        glm::vec2 PrevPosition;
        glm::vec2 Velocity;
        glm::vec2 Force;
        float Mass;
        bool Fixed;

        FEMNode(const glm::vec2& pos, float mass = 1.0f, bool fixed = false)
            : Position(pos), PrevPosition(pos), Velocity(0.0f), Force(0.0f)
            , Mass(mass), Fixed(fixed)
        {
        }
    };

    // 2D 三角形单元结构
    struct TriangleElement
    {
        int NodeIndices[3]; // 三个顶点的索引
        float YoungsModulus; // 杨氏模量 (刚度)
        float PoissonRatio;  // 泊松比 (压缩性)
        float Thickness;     // 厚度 (用于平面应力/应变)

        // 用于计算变形梯度的初始几何参数 (参考形状)
        glm::mat2 InverseReferenceMatrix;
        float ReferenceArea;

        TriangleElement(int n0, int n1, int n2, float E = 10000.0f, float nu = 0.3f, float thickness = 1.0f)
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

        // --- 构建器方法 ---
        void AddNode(const glm::vec2& position, float mass = 1.0f, bool fixed = false)
        {
            m_Nodes.emplace_back(position, mass, fixed);
        }

        // 添加三角形单元，并自动计算参考几何信息
        void AddTriangle(int n0, int n1, int n2, float stiffness = 10000.0f, float poisson = 0.3f)
        {
            TriangleElement elem(n0, n1, n2, stiffness, poisson);

            // 计算参考构型下的矩阵 D_m 和面积
            // D_m = [x1-x0, x2-x0]
            glm::vec2 X0 = m_Nodes[n0].Position;
            glm::vec2 X1 = m_Nodes[n1].Position;
            glm::vec2 X2 = m_Nodes[n2].Position;

            glm::mat2 Dm(X1 - X0, X2 - X0); // 列向量构造矩阵

            elem.ReferenceArea = 0.5f * glm::determinant(Dm);

            // 防止除零
            if (std::abs(elem.ReferenceArea) > 1e-6f) {
                elem.InverseReferenceMatrix = glm::inverse(Dm);
            }
            else {
                elem.InverseReferenceMatrix = glm::mat2(0.0f);
            }

            m_Elements.push_back(elem);
        }

        // --- 访问器 ---
        void SetNodePosition(int i, const glm::vec2& pos) { m_Nodes[i].Position = pos; }
        glm::vec2 GetNodePosition(int i) { return m_Nodes[i].Position; }
        glm::vec2 GetNodeVelocity(int i) { return m_Nodes[i].Velocity; }

        bool GetInitialize() { return m_IsInitialized; }
        void SetInitialize(bool bo) { m_IsInitialized = bo; }
        operator bool() { return m_IsInitialized; }

        // --- 核心循环 ---
        void OnAttach();
        void OnUpdate(float dt); // 这里简化直接用 float dt，也可以换成 Timestep

        void ComputeForces();
        void Integrate(float dt);


        bool m_IsInitialized = false;
        std::vector<FEMNode> m_Nodes;
        std::vector<TriangleElement> m_Elements;

        glm::vec2 m_Gravity = glm::vec2(0.0f, -9.8f);
        float m_Damping = 0.05f; // 简单的全局阻尼
    };

}