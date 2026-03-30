#include "rgpch.h"
#include "Scene.h"
#include"Resug/Renderer/Renderer2D.h"

#include"Entity.h"
#include"Components.h"

namespace Resug
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(std::string name)
	{
		Entity entity = { m_Registry.create() ,this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>(name);
		
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		Entity e{};

		auto view = m_Registry.view< CameraComponent>();
		view.each([&](auto entity, CameraComponent& camera)
			{
				if (camera.Primary)
				{
					e =  Entity{ entity ,this };
					return;
				}
			});
		return e;
	}

	void Scene::OnUpdateEditor(Timestep ts)
	{
		/////////////////////////////SpriteRendererComponent////////////////////
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				glm::vec4 Triangle1[3] = {
					glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
					glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
					glm::vec4(0.5f,  0.5f, 0.0f, 1.0f)
				};
				glm::vec4 Triangle2[3] = {
					glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
					glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f),
					glm::vec4(0.5f,  0.5f, 0.0f, 1.0f)
				};
				Renderer2D::DrawTriangle(transform, sprite.Color, Triangle1);
				Renderer2D::DrawTriangle(transform, sprite.Color, Triangle2);

			}
		}

		////////////////////////////MeshRendererComponent/////////////////

		{
			auto view = m_Registry.view<TransformComponent, MeshRendererComponent>();
			view.each([&](auto entity, TransformComponent& transform, MeshRendererComponent& meshCom)
				{
					auto& mesh = meshCom.Mesh;
					uint32_t unitWidth = mesh.GetWidth();
					uint32_t unitHeight = mesh.GetHeight();

					mesh.CalculateVertexPositionByRelative(transform.Position);
					
					for (int i = 0; i < unitHeight-1; i++)
					{
						for (int j = 0; j < unitWidth-1; j++)
						{
							//glm::vec4 average = mesh.CalculateAveragePosition();
							//transform.Position = glm::vec3(average.x, average.y, average.z);
							//transform.RecalculateTransform();
							//mesh.CalculateRelative(glm::vec3(0.0f));

							glm::vec4 l1 = mesh.GetRelativePosition(j  , i);
							glm::vec4 r1 = mesh.GetRelativePosition(j + 1 , i);
							glm::vec4 r2 = mesh.GetRelativePosition(j + 1  , i + 1);
							glm::vec4 l2 = mesh.GetRelativePosition(j  , i + 1);
							glm::vec4 QuadVertex[4] = { l1, r1, r2, l2 };

							
							//std::cout << transform << "\n";
							//std::cout << l1<<l2<<r1<<r2 << "\n";
							Renderer2D::DrawQuad(transform, mesh.GetColor(), QuadVertex);
						}
					}
				});
		}

		Renderer2D::EndScene();

	}

	void Scene::OnUpdateSimulation(Timestep ts)
	{
		//////////////////BoxCollider2DComponent//////
		{
			auto view = m_Registry.view<TransformComponent, BoxCollider2DComponent>();
			view.each([&](auto entity, TransformComponent& transform, BoxCollider2DComponent& bc)
				{
					bc.SetVertexPosition(transform.Position);
				});
		}
		//rigidbody/////BoxCollider2DComponent//////
		{
			auto view = m_Registry.view<TransformComponent, BoxCollider2DComponent, RigidBodyComponent>();
			view.each([&](auto entity, TransformComponent& transform, BoxCollider2DComponent& bc, RigidBodyComponent& rb)
				{
					rb.rb.CalculateVelocity(ts);
					glm::vec3 entityVelocity =  rb.rb.GetVelocity();

					//std::cout << " Scene::OnUpdateSimulation" << "\n";
					bc.SetVertexPosition(transform.Position);

					transform.Position += bc.OnUpdate(ts, entityVelocity);
					transform.RecalculateTransform();

				});
		}
		///////////////MeshCollider2DComponent//////
		{
			auto view = m_Registry.view<MeshRendererComponent , MeshCollider2DComponent>();
			view.each([&](auto entity, MeshRendererComponent& mesherendercom, MeshCollider2DComponent& meshcom)
				{
					auto& collider = meshcom.MeshCollider;
					auto& render = mesherendercom.Mesh;
					collider.SetVertexSize(render.GetHeight() * render.GetWidth());
					uint32_t size = collider.m_VertexSize;
					for (int i = 0; i < collider.m_VertexSize; i++)
					{
						collider.m_VertexPosition[i] = render.m_VertexPosition[i];
					}


					//std::cout << render.m_VertexPosition[0] << "\n";
				});
		}
		///////////////SMS2DComponent//////
		{
			auto view = m_Registry.view<MeshRendererComponent, MeshCollider2DComponent,SMS2DComponent>();
			view.each([&](auto entity, MeshRendererComponent& mesherendercom, MeshCollider2DComponent& meshcom, SMS2DComponent& smsCom)
				{
					auto& collider = meshcom.MeshCollider;
					auto& render = mesherendercom.Mesh;
					auto& sms = smsCom.SMS;

					collider.SetVertexSize(render.GetHeight() * render.GetWidth());
					uint32_t size = collider.m_VertexSize;
					
					if (size <= 0) { return; }
					if (!sms)
					{
						sms.SetIntialize(true);
						for (int i = 0; i < size; i++)
						{
							sms.AddPoint(render.GetVertexPosition(i));
						}
						uint32_t unitWidth = render.GetWidth();
						uint32_t unitHeight = render.GetHeight();


						for (int i = 0; i < unitHeight; i++)
						{
							for (int j = 0; j < unitWidth; j++)
							{
								
								uint32_t index = i * unitWidth + j;
								
								if (j != unitWidth - 1)sms.AddSpring(index, index + 1);
								else if (i != unitHeight - 1)sms.AddSpring(index, index + unitWidth);
								if (j != unitWidth - 1 && i != unitHeight - 1)
								{
									sms.AddSpring(index, index + unitWidth + 1);
									sms.AddSpring(index + 1 , index + unitWidth);
								
								}
							}
						}
					}
					for (int i = 0; i < size; i++)
					{
						sms.SetPointPosition(i, render.GetVertexPosition(i));

						collider.m_VertexPosition[i] = render.m_VertexPosition[i];//TODO:改成函数
					}


					glm::vec3 velocityArray[10000];
					for (int i = 0; i < 10; i++)
					{
						sms.OnUpdate(ts);
				
						for (int i = 0; i < collider.m_VertexSize; i++)
						{
							velocityArray[i] = sms.GetPointVelocity(i);
						}

						collider.OnUpdate(ts, velocityArray);

						for (int i = 0; i < size; i++)
						{
							sms.SetPointPosition(i, collider.GetVertexPosition(i));
						}
					}


					for (int i = 0; i < collider.m_VertexSize; i++)
					{
						render.m_VertexPosition[i] = glm::vec4(collider.m_VertexPosition[i], 1.0f);
					}
					//std::cout << render.m_VertexPosition[0] << "\n";
				});
		}

		///////////////FEM2DComponent//////
		{
			auto view = m_Registry.view<MeshRendererComponent, MeshCollider2DComponent, FEM2DComponent>();
			view.each([&](auto entity, MeshRendererComponent& mesherendercom, MeshCollider2DComponent& meshcom, FEM2DComponent& femCom)
				{
					auto& collider = meshcom.MeshCollider;
					auto& render = mesherendercom.Mesh;
					auto& fem = femCom.FEM;

					collider.SetVertexSize(render.GetHeight() * render.GetWidth());
					uint32_t size = collider.m_VertexSize;
					if (size <= 0) { return; }

					if (!fem)
					{
						std::cout << "INit\n";
						fem.SetInitialize(true);

						// 1. 添加节点
						for (int i = 0; i < size; i++)
						{
							glm::vec4 pos4 = render.GetVertexPosition(i);
							fem.AddNode(glm::vec2(pos4.x, pos4.y));
						}

						uint32_t unitWidth = render.GetWidth();
						uint32_t unitHeight = render.GetHeight();

						// 2. 添加三角形
						for (int i = 0; i < unitHeight - 1; i++)
						{
							for (int j = 0; j < unitWidth - 1; j++)
							{
								uint32_t A = i * unitWidth + j;
								uint32_t B = i * unitWidth + (j + 1);
								uint32_t C = (i + 1) * unitWidth + j;
								uint32_t D = (i + 1) * unitWidth + (j + 1);

								// 稍微降低初始刚度以防万一，或者确保子步够多
								fem.AddTriangle(A, B, C, 5000.0f, 0.3f);
								fem.AddTriangle(B, D, C, 5000.0f, 0.3f);
							}
						}

						// 3. 【核心修复】强制刷新参考构型
						// 必须确保 FEM 内部记录的“初始形状”与“当前形状”完全一致
						for (auto& elem : fem.m_Elements)
						{
							const FEMNode& n0 = fem.m_Nodes[elem.NodeIndices[0]];
							const FEMNode& n1 = fem.m_Nodes[elem.NodeIndices[1]];
							const FEMNode& n2 = fem.m_Nodes[elem.NodeIndices[2]];

							glm::mat2 Dm(n1.Position - n0.Position, n2.Position - n0.Position);
							// 使用 abs 防止顶点顺序导致的负面积
							elem.ReferenceArea = 0.5f * std::abs(glm::determinant(Dm));

							if (std::abs(elem.ReferenceArea) > 1e-6f) {
								elem.InverseReferenceMatrix = glm::inverse(Dm);
							}
						}
					}

					// 4. 同步位置 (如果是从外部同步)
					for (int i = 0; i < size; i++)
					{
						glm::vec4 pos4 = render.GetVertexPosition(i);
						fem.SetNodePosition(i, glm::vec2(pos4.x, pos4.y));
						collider.m_VertexPosition[i] = render.m_VertexPosition[i];
					}

					// 5. 物理更新
					glm::vec3 velocityArray[10000];
					int subSteps = 10; // 如果还爆炸，增加到 20
					float dt = ts;
					//dt /= 10.0f;
					for (int s = 0; s < subSteps; s++)
					{
						fem.ComputeForces();
						fem.Integrate(dt / subSteps);

						for (int i = 0; i < size; i++)
						{
							glm::vec2 femVel = fem.GetNodeVelocity(i);
							velocityArray[i] = glm::vec3(femVel.x, femVel.y, 0.0f);
						}
						collider.OnUpdate(ts, velocityArray);

						for (int i = 0; i < size; i++)
						{
							fem.SetNodePosition(i, collider.GetVertexPosition(i));
						}

					}

					// 6. 回写数据
					//for (int i = 0; i < size; i++)
					//{
					//	glm::vec2 femPos = fem.GetNodePosition(i);
					//	glm::vec2 femVel = fem.GetNodeVelocity(i);

					//	collider.m_VertexPosition[i] = glm::vec3(femPos.x, femPos.y, 0.0f);
					//	velocityArray[i] = glm::vec3(femVel.x, femVel.y, 0.0f);
					//}

					//collider.OnUpdate(ts, velocityArray);

					for (int i = 0; i < size; i++)
					{
						render.m_VertexPosition[i] = glm::vec4(collider.m_VertexPosition[i], 1.0f);
					}
				});
		}
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		glm::mat4 mainCameraProjection = glm::mat4(1.0f);
		glm::mat4 cameraTransform  = glm::mat4(0.0f);
		{
			
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			int i = 0;
			view.each([&](auto entity, TransformComponent& transform, CameraComponent& camera)
				{
					if (camera.Primary)
					{
						mainCameraProjection = camera.Camera.GetProjection();
						cameraTransform = transform;
						i++;
						
					}
				});
		}

		if (mainCameraProjection != glm::mat4(1.0f))
		{
			Renderer2D::BeginScene(mainCameraProjection, cameraTransform);
		}
		//Sprite///////////
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				glm::vec4 Triangle1[3] = {
					glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
					glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
					glm::vec4(0.5f,  0.5f, 0.0f, 1.0f)
				};
				glm::vec4 Triangle2[3] = {
					glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
					glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f),
					glm::vec4(0.5f,  0.5f, 0.0f, 1.0f)
				};
				Renderer2D::DrawTriangle(transform, sprite.Color, Triangle1);
				Renderer2D::DrawTriangle(transform, sprite.Color, Triangle2);

			}
		}
		////////////////////////////MeshRendererComponent/////////////////

		{
			auto view = m_Registry.view<TransformComponent, MeshRendererComponent>();
			view.each([&](auto entity, TransformComponent& transform, MeshRendererComponent& meshCom)
				{
					auto& mesh = meshCom.Mesh;
					uint32_t unitWidth = mesh.GetWidth();
					uint32_t unitHeight = mesh.GetHeight();

					for (int i = 0; i < unitHeight - 1; i++)
					{
						for (int j = 0; j < unitWidth - 1; j++)
						{
							glm::vec4 average = mesh.CalculateAveragePosition();
							transform.Position = glm::vec3(average.x, average.y, average.z);
							transform.RecalculateTransform();
							mesh.CalculateRelative(transform.Position);

							glm::vec4 v00 = mesh.GetRelativePosition(j, i);
							glm::vec4 v10 = mesh.GetRelativePosition(j + 1, i);
							glm::vec4 v11 = mesh.GetRelativePosition(j + 1, i + 1);
							glm::vec4 v01 = mesh.GetRelativePosition(j, i + 1);

							// 渲染三角形1 (v00, v10, v01)
							glm::vec4 tri1[3] = { v00, v10, v01 };
							Renderer2D::DrawTriangle(transform, mesh.GetColor(), tri1);

							// 渲染三角形2 (v10, v11, v01)
							glm::vec4 tri2[3] = { v10, v11, v01 };
							Renderer2D::DrawTriangle(transform, mesh.GetColor(), tri2);
						}
					}
				});
		}
		//NativeScriptComponent////////////////
		{

			auto view = m_Registry.view<NativeScriptComponent>();
			view.each([&](auto entity, NativeScriptComponent& ns)
				{
					if (ns.Instance == nullptr)
					{
						ns.InstanceFunction();
						ns.Instance->m_Entity = Entity{ entity ,this};

						ns.OnCreateFunction(ns.Instance);
					}

					ns.OnUpdateFunction(ns.Instance, ts);
				});
		}

		Renderer2D::EndScene();
	}
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		{

			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			
			view.each([&](auto entity, TransformComponent& transform, CameraComponent& camera)
				{
					if (!camera.Fixed)
						camera.Camera.SetViewportSize(width, height);
				});

		}
	}
}