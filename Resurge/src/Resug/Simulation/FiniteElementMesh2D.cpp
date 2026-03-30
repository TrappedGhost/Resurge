#include "rgpch.h"
#include "FiniteElementMesh2D.h"

namespace Resug {

    void FEMSystem2D::OnAttach()
    {
        // 初始化逻辑，如果需要可以在这里预计算
        // 例如：重新计算所有单元的质量分配
        m_IsInitialized = true;
    }

    void FEMSystem2D::OnUpdate(float dt)
    {
        // 限制时间步长以保证稳定性 (FEM 对 dt 很敏感)
        float subSteps = 5.0f;
        float subDt = dt / subSteps;

        for (int i = 0; i < subSteps; i++)
        {
            ComputeForces();
            Integrate(subDt);
        }
    }

    void FEMSystem2D::ComputeForces()
    {
        // 1. 重置力
        for (auto& node : m_Nodes)
        {
            node.Force = glm::vec2(0.0f);
        }

        // 2. 应用重力
        for (auto& node : m_Nodes)
        {
            if (!node.Fixed)  // 固定节点不受重力
                node.Force += m_Gravity * node.Mass;
        }

        // 3. 计算单元内力
        for (const auto& elem : m_Elements)
        {
            // 获取当前节点位置
            const FEMNode& n0 = m_Nodes[elem.NodeIndices[0]];
            const FEMNode& n1 = m_Nodes[elem.NodeIndices[1]];
            const FEMNode& n2 = m_Nodes[elem.NodeIndices[2]];

            // 构建当前构型矩阵 Ds = [x1-x0, x2-x0]
            glm::mat2 Ds;
            Ds[0] = n1.Position - n0.Position;
            Ds[1] = n2.Position - n0.Position;

            // 计算变形梯度 F = Ds * inv(Dm)
            glm::mat2 F = Ds * elem.InverseReferenceMatrix;

            // 计算格林应变 E = 0.5 * (F^T * F - I)
            glm::mat2 Ft = glm::transpose(F);
            glm::mat2 FtF = Ft * F;
            glm::mat2 I(1.0f);
            glm::mat2 E = 0.5f * (FtF - I);

            // Lame参数
            float lambda = (elem.YoungsModulus * elem.PoissonRatio) /
                ((1.0f + elem.PoissonRatio) * (1.0f - 2.0f * elem.PoissonRatio));
            float mu = elem.YoungsModulus / (2.0f * (1.0f + elem.PoissonRatio));

            // 第二皮奥拉-基尔霍夫应力 S = lambda * tr(E) * I + 2 * mu * E
            float traceE = E[0][0] + E[1][1];
            glm::mat2 S = lambda * traceE * I + 2.0f * mu * E;

            // 第一皮奥拉-基尔霍夫应力 P = F * S
            glm::mat2 P = F * S;

            // 正确的形状函数梯度计算
            // 对于线性三角形，梯度在参考构型中是常数
            // [dN0/dX, dN1/dX, dN2/dX] = inv(Dm)^T，其中 N0+N1+N2=1
            glm::mat2 invDmT = glm::transpose(elem.InverseReferenceMatrix);

            // 形状函数的梯度（在参考坐标系中）
            glm::vec2 gradN1 = invDmT[0];  // dN1/dX
            glm::vec2 gradN2 = invDmT[1];  // dN2/dX
            glm::vec2 gradN0 = -gradN1 - gradN2;  // dN0/dX = -(dN1/dX + dN2/dX)

            // 计算节点力（注意负号：内力抵抗变形）
            float volume = elem.ReferenceArea * elem.Thickness;

            // 力 = -volume * P * gradN
            glm::vec2 f0 = -volume * (P * gradN0);
            glm::vec2 f1 = -volume * (P * gradN1);
            glm::vec2 f2 = -volume * (P * gradN2);

            // 累加力
            m_Nodes[elem.NodeIndices[0]].Force += f0;
            m_Nodes[elem.NodeIndices[1]].Force += f1;
            m_Nodes[elem.NodeIndices[2]].Force += f2;
        }

        // 4. 添加阻尼力（重要！防止爆炸）
        for (auto& node : m_Nodes)
        {
            if (!node.Fixed)
            {
                node.Force -= m_Damping * node.Velocity;
            }
        }
    }

    void FEMSystem2D::Integrate(float dt)
    {
        for (auto& node : m_Nodes)
        {
            if (node.Fixed) continue;

            // 简单的显式欧拉积分
            // a = F / m
            glm::vec2 acceleration = node.Force / node.Mass;

            // 速度 Verlet 或者简单的阻尼欧拉
            node.Velocity += acceleration * dt;

            // 应用全局阻尼
            node.Velocity *= (1.0f - m_Damping);

            //node.Position += node.Velocity * dt;
             //node.PrevPosition = node.Position - node.Velocity * dt; // 仅用于记录
        }
    }

}