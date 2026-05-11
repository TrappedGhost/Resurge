// SpringMassSystem.h
#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "MassPoint.h"
#include "Spring.h"

namespace Resug {


    class SpringMassSystem
    {
    public:
        SpringMassSystem() = default;

        void AddPoint(const glm::dvec3& position, double mass = 1.0f, bool fixed = false)
        {
            m_Points.emplace_back(position, mass, fixed);
        }

        void AddSpring(int a, int b, double stiffness = 500.0f, double damping = 2.0f)
        {
            double restLength = glm::distance(m_Points[a].Position, m_Points[b].Position);
            m_Springs.emplace_back(a, b, restLength, stiffness, damping);
        }

        // 创建矩形布料网格
        void CreateClothGrid(int width, int height, double spacing,
            double stiffness = 500.0f, double damping = 2.0f)
        {
            // 创建质点
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    glm::dvec3 pos((x - width / 2.0f) * spacing,
                        y * spacing + 5.0f,  // 从高度5开始
                        0.0f);
                    // 固定顶部两个角
                    bool fixed = (y == height - 1 && (x == 0 || x == width - 1));
                    AddPoint(pos, 0.1f, fixed);
                }
            }

            // 创建弹簧（结构弹簧 + 剪切弹簧 + 弯曲弹簧）
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    int idx = y * width + x;

                    // 结构弹簧（上下左右）
                    if (x + 1 < width)
                        AddSpring(idx, y * width + (x + 1), stiffness, damping);
                    if (y + 1 < height)
                        AddSpring(idx, (y + 1) * width + x, stiffness, damping);

                    // 剪切弹簧（对角线）
                    if (x + 1 < width && y + 1 < height)
                        AddSpring(idx, (y + 1) * width + (x + 1), stiffness * 0.7f, damping);
                    if (x - 1 >= 0 && y + 1 < height)
                        AddSpring(idx, (y + 1) * width + (x - 1), stiffness * 0.7f, damping);

                    // 弯曲弹簧（隔一个点）
                    if (x + 2 < width)
                        AddSpring(idx, y * width + (x + 2), stiffness * 0.5f, damping);
                    if (y + 2 < height)
                        AddSpring(idx, (y + 2) * width + x, stiffness * 0.5f, damping);
                }
            }

            isIntialize = true;
        }

        void OnUpdate(double dt)
        {
            if (!isIntialize) return;

            ComputeForces();
            IntegrateSemiImplicitEuler(dt);
            HandleGroundCollision();
        }

        // 获取点数量
        size_t GetPointCount() const { return m_Points.size(); }

        // 获取点位置（用于渲染）
        const glm::dvec3& GetPointPosition(int i) const { return m_Points[i].Position; }

        // 获取弹簧数量
        size_t GetSpringCount() const { return m_Springs.size(); }

        // 获取弹簧端点
        void GetSpringEndpoints(int i, int& a, int& b) const
        {
            a = m_Springs[i].PointA;
            b = m_Springs[i].PointB;
        }

        bool GetIntialize() const { return isIntialize; }

    private:
        void ComputeForces();
        void IntegrateSemiImplicitEuler(double dt);
        void HandleGroundCollision();

    private:
        bool isIntialize = false;
        std::vector<MassPoint> m_Points;
        std::vector<Spring> m_Springs;

        glm::dvec3 m_Gravity = glm::dvec3(0.0f, -9.8f, 0.0f);
       
        double m_GroundY = -3.0f;              // 地面高度
        double m_Restitution = 0.7f;           // 弹跳系数（0.7 = 弹起70%高度）
    };
}