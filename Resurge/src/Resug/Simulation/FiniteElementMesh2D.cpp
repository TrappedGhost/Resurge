#include "rgpch.h"
#include "FiniteElementMesh2D.h"

namespace Resug {

    void FEMSystem2D::OnAttach()
    {
        m_IsInitialized = true;
    }

    void FEMSystem2D::OnUpdate(double dt)
    {
        double subSteps = 5.0f;
        double subDt = dt / subSteps;

        for (int i = 0; i < subSteps; i++)
        {
            ComputeForces();
            IntegrateImplicitNewton(subDt);
        }
    }

    void FEMSystem2D::ComputeForces()
    {
        for (auto& node : m_Nodes)
        {
            node.Force = glm::dvec2(0.0f);
        }

        for (auto& node : m_Nodes)
        {
            if (!node.Fixed)  // 固定节点不受重力
                node.Force += m_Gravity * node.Mass;
        }

        // 计算单元内力
        for (const auto& elem : m_Elements)
        {
            // 获取当前节点位置
            const FEMNode& n0 = m_Nodes[elem.NodeIndices[0]];
            const FEMNode& n1 = m_Nodes[elem.NodeIndices[1]];
            const FEMNode& n2 = m_Nodes[elem.NodeIndices[2]];

            // 构建当前构型矩阵 Ds = [x1-x0, x2-x0]
            glm::dmat2 Ds;
            Ds[0] = n1.Position - n0.Position;
            Ds[1] = n2.Position - n0.Position;

            // 计算变形梯度 F = Ds * inv(Dm)
            glm::dmat2 F = Ds * elem.InverseReferenceMatrix;

            // 计算格林应变 E = 0.5 * (F^T * F - I)
            glm::dmat2 Ft = glm::transpose(F);
            glm::dmat2 FtF = Ft * F;
            glm::dmat2 I(1.0);
            glm::dmat2 E = 0.5 * (FtF - I);

            // Lame参数
            double lambda = (elem.YoungsModulus * elem.PoissonRatio) /
                ((1.0f + elem.PoissonRatio) * (1.0f - 2.0f * elem.PoissonRatio));
            double mu = elem.YoungsModulus / (2.0f * (1.0f + elem.PoissonRatio));

            // 第二皮奥拉-基尔霍夫应力 S = lambda * tr(E) * I + 2 * mu * E
            double traceE = E[0][0] + E[1][1];
            glm::dmat2 S = lambda * traceE * I + 2.0f * mu * E;

            // 第一皮奥拉-基尔霍夫应力 P = F * S
            glm::dmat2 P = F * S;

            // 正确的形状函数梯度计算
            // 对于线性三角形，梯度在参考构型中是常数
            // [dN0/dX, dN1/dX, dN2/dX] = inv(Dm)^T，其中 N0+N1+N2=1
            glm::dmat2 invDmT = glm::transpose(elem.InverseReferenceMatrix);

            // 形状函数的梯度（在参考坐标系中）
            glm::dvec2 gradN1 = invDmT[0];  // dN1/dX
            glm::dvec2 gradN2 = invDmT[1];  // dN2/dX
            glm::dvec2 gradN0 = -gradN1 - gradN2;  // dN0/dX = -(dN1/dX + dN2/dX)

            // 计算节点力（注意负号：内力抵抗变形）
            double volume = elem.ReferenceArea * elem.Thickness;

            // 力 = -volume * P * gradN
            glm::dvec2 f0 = -volume * (P * gradN0);
            glm::dvec2 f1 = -volume * (P * gradN1);
            glm::dvec2 f2 = -volume * (P * gradN2);

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

    void FEMSystem2D::Integrate(double dt)
    {
        for (auto& node : m_Nodes)
        {
            if (node.Fixed) continue;

            // 简单的显式欧拉积分
            // a = F / m
            glm::dvec2 acceleration = node.Force / node.Mass;

            // 速度 Verlet 或者简单的阻尼欧拉
            node.Velocity += acceleration * dt;

            // 应用全局阻尼
            node.Velocity *= (1.0f - m_Damping);

            //node.Position += node.Velocity * dt;
             //node.PrevPosition = node.Position - node.Velocity * dt; // 仅用于记录
        }
    }
    void FEMSystem2D::IntegrateImplicitNewton(double dt)
    {
        if (dt > 0.033f) dt = 0.033f;

        for (auto& node : m_Nodes)
        {
            if (node.Fixed) continue;

            // 半隐式欧拉：先更新速度，再更新位置
            glm::dvec2 acceleration = node.Force / node.Mass;

            // 速度更新
            node.Velocity += acceleration * dt;

            // 阻尼：使用指数衰减
            node.Velocity *= (1.0f - m_Damping * dt);

            // 限制最大速度（防止爆炸）
            double maxSpeed = 10.0f;
            if (glm::length(node.Velocity) > maxSpeed)
            {
                node.Velocity = glm::normalize(node.Velocity) * maxSpeed;
            }

            // 位置更新（使用新速度）
            node.Position += node.Velocity * dt;

            // 边界限制（可选）
            if (node.Position.y < -10.0f)
            {
                node.Position.y = -10.0f;
                node.Velocity.y = -node.Velocity.y * 0.5f;
            }
        }
    }

}