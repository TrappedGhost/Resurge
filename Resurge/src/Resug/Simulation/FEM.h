#pragma once
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Resug
{
    class FiniteElementMesh2D {
    public:
        // 节点结构
        struct Node {
            glm::vec2 position;      // 当前位置
            glm::vec2 velocity;      // 当前速度
            glm::vec2 force;         // 合力
            float mass;              // 质量
            bool fixed;              // 是否固定

            Node() : position(0.0f), velocity(0.0f), force(0.0f), mass(1.0f), fixed(false) {}
            Node(const glm::vec2& pos, float m = 1.0f)
                : position(pos), velocity(0.0f), force(0.0f), mass(m), fixed(false) {
            }
        };

        // 三角形单元
        struct Triangle {
            int nodes[3];            // 三个节点的索引
            float restArea;          // 初始面积
            glm::mat2 invDm;         // 初始形状矩阵的逆 (2x2)

            Triangle() : restArea(0.0f) {
                nodes[0] = nodes[1] = nodes[2] = -1;
            }
        };

    private:
        std::vector<Node> m_nodes;           // 所有节点
        std::vector<Triangle> m_triangles;    // 所有三角形
        std::vector<glm::vec2> m_forces;      // 力缓存

        // 物理参数
        float m_youngsModulus;     // 杨氏模量
        float m_poissonRatio;      // 泊松比
        float m_damping;           // 阻尼系数

        // 临时的刚度矩阵（用于隐式积分）
        std::vector<float> m_stiffnessMatrix;  // 简化的稀疏存储

    public:
        FiniteElementMesh2D()
            : m_youngsModulus(1000.0f)
            , m_poissonRatio(0.3f)
            , m_damping(0.01f) {
        }

        // 设置物理参数
        void setYoungsModulus(float value) { m_youngsModulus = value; }
        void setPoissonRatio(float value) { m_poissonRatio = value; }
        void setDamping(float value) { m_damping = value; }

        // 添加节点
        int addNode(const glm::vec2& position, float mass = 1.0f) {
            m_nodes.emplace_back(position, mass);
            m_forces.resize(m_nodes.size());
            return m_nodes.size() - 1;
        }

        // 添加三角形单元
        void addTriangle(int n1, int n2, int n3) {
            Triangle tri;
            tri.nodes[0] = n1;
            tri.nodes[1] = n2;
            tri.nodes[2] = n3;

            // 计算初始形状矩阵和面积
            glm::vec2 a = m_nodes[n1].position;
            glm::vec2 b = m_nodes[n2].position;
            glm::vec2 c = m_nodes[n3].position;

            // 形状矩阵 Dm = [b-a, c-a] (2x2)
            glm::mat2 Dm;
            Dm[0] = b - a;
            Dm[1] = c - a;

            // 计算面积 = |det(Dm)| / 2
            tri.restArea = 0.5f * std::abs(glm::determinant(Dm));

            // 存储逆矩阵
            tri.invDm = glm::inverse(Dm);

            m_triangles.push_back(tri);
        }

        // 固定节点
        void setNodeFixed(int index, bool fixed = true) {
            if (index >= 0 && index < m_nodes.size()) {
                m_nodes[index].fixed = fixed;
            }
        }

        // 获取节点引用（方便直接修改）
        Node& getNode(int index) { return m_nodes[index]; }
        const Node& getNode(int index) const { return m_nodes[index]; }

        // 获取所有节点
        const std::vector<Node>& getNodes() const { return m_nodes; }
        std::vector<Node>& getNodes() { return m_nodes; }

        // 计算内力（线弹性模型）
        void computeInternalForces() {
            // 清零力
            for (auto& node : m_nodes) {
                node.force = glm::vec2(0.0f);
            }
            //计算重力

            glm::vec2 m_gravity = glm::vec2(0.0f, -9.8f);
            for (auto& node : m_nodes) {
                if (!node.fixed) {  // 固定节点不受重力影响（可选）
                    node.force += node.mass * m_gravity;
                }
            }

            // 遍历每个三角形
            for (const auto& tri : m_triangles) {
                // 获取三个节点的当前位置
                const glm::vec2& a = m_nodes[tri.nodes[0]].position;
                const glm::vec2& b = m_nodes[tri.nodes[1]].position;
                const glm::vec2& c = m_nodes[tri.nodes[2]].position;

                // 当前形状矩阵 Ds = [b-a, c-a]
                glm::mat2 Ds;
                Ds[0] = b - a;
                Ds[1] = c - a;

                // 变形梯度 F = Ds * invDm
                glm::mat2 F = Ds * tri.invDm;

                // 格林应变 E = 1/2 * (F^T * F - I)
                glm::mat2 FtF = glm::transpose(F) * F;
                glm::mat2 E = 0.5f * (FtF - glm::mat2(1.0f));

                // 线弹性本构：应力 P = 2*mu*F*E + lambda*tr(E)*F
                float mu = m_youngsModulus / (2.0f * (1.0f + m_poissonRatio));
                float lambda = m_youngsModulus * m_poissonRatio /
                    ((1.0f + m_poissonRatio) * (1.0f - 2.0f * m_poissonRatio));

                // 第一Piola-Kirchhoff应力
                float traceE = E[0][0] + E[1][1];  // 手动计算迹（2x2矩阵的对角线之和）
                glm::mat2 P = 2.0f * mu * F * E + lambda * traceE * F;

                // 计算节点力 (使用公式 f = -volume * P * invDm^T)
                float volume = tri.restArea;  // 二维中"体积"就是面积
                glm::mat2 H = -volume * P * glm::transpose(tri.invDm);

                // 分配节点力 (力的方向与形状函数导数相关)
                glm::vec2 f1(0.0f), f2(0.0f), f3(0.0f);

                // 对于线性三角形，力的分配公式
                f2 = glm::vec2(H[0][0], H[1][0]);  // H的列对应节点
                f3 = glm::vec2(H[0][1], H[1][1]);
                f1 = -f2 - f3;

                // 累加到节点
                m_nodes[tri.nodes[0]].force += f1;
                m_nodes[tri.nodes[1]].force += f2;
                m_nodes[tri.nodes[2]].force += f3;
            }

            // 添加阻尼力 (与速度成正比)
            //for (auto& node : m_nodes) {
            //    node.force -= m_damping * node.velocity;
            //}
        }

        // 隐式积分（简化版）
        void implicitIntegrate(float dt) {

            dt /= 100.0f;
            for (int i = 0; i < 10; i++)
            {
                int n = m_nodes.size();
                if (n == 0) return;

                // 计算当前力
                computeInternalForces();

                // 存储当前力作为外力
                std::vector<glm::vec2> externalForces(n);
                for (int i = 0; i < n; i++) {
                    externalForces[i] = m_nodes[i].force;
                }

                // 简化的隐式欧拉：用当前力的近似求解
                // 实际应该构建刚度矩阵，这里用简化版本
                for (int i = 0; i < n; i++) {
                    if (m_nodes[i].fixed) continue;

                    // 简化的隐式更新 (实际应该是求解线性系统)
                    glm::vec2 acceleration = externalForces[i] / m_nodes[i].mass;
                    m_nodes[i].velocity += dt * acceleration;
                    m_nodes[i].position += dt * m_nodes[i].velocity;
                }

                float m_groundY = -2.0f;  // 地面高度

                for (auto& node : m_nodes) {
                    if (node.position.y < m_groundY) {
                        node.position.y = m_groundY;
                        node.velocity.y = -node.velocity.y * 0.3f;  // 反弹
                    }
                }
            }

        }

        // 显式积分（用于对比）
        void explicitIntegrate(float dt) {
            computeInternalForces();

            for (auto& node : m_nodes) {
                if (node.fixed) continue;

                glm::vec2 acceleration = node.force / node.mass;
                node.velocity += dt * acceleration;
                node.position += dt * node.velocity;
            }
        }

        // 重置到初始位置
        void resetToRest() {
            // 需要存储初始位置，这里简化处理
            // 实际使用中应该在addNode时保存rest position
        }

        // 获取节点数量
        int getNodeCount() const { return m_nodes.size(); }

        // 获取三角形数量
        int getTriangleCount() const { return m_triangles.size(); }

        // 调试：输出节点信息
        void printNodeInfo(int index) const {
            if (index >= 0 && index < m_nodes.size()) {
                const auto& n = m_nodes[index];
                printf("Node %d: pos=(%.2f,%.2f) vel=(%.2f,%.2f) fixed=%d\n",
                    index, n.position.x, n.position.y, n.velocity.x, n.velocity.y, n.fixed);
            }
        }
    };
}