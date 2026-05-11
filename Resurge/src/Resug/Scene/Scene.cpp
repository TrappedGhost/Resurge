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

				Resug::SpriteRendererComponent::SpriteTpye switch_on = sprite.Type;

				switch (switch_on)
				{
				case Resug::SpriteRendererComponent::SpriteTpye::Quad:
				{
					glm::vec4 Triangle1[3] =
					{
						glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
						glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
						glm::vec4(0.5f,  0.5f, 0.0f, 1.0f)
					};
					glm::vec4 Triangle2[3] =
					{
						glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
						glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f),
						glm::vec4(0.5f,  0.5f, 0.0f, 1.0f)
					};
					Renderer2D::DrawTriangle(glm::mat4(transform.GetTransform()), sprite.Color,Triangle1);
					Renderer2D::DrawTriangle(glm::mat4(transform.GetTransform()), sprite.Color, Triangle2);
					break;
				}
				case Resug::SpriteRendererComponent::SpriteTpye::Circle:
				{
					float radius = sprite.radius;
					glm::dvec4 center = glm::dvec4(0.0f, 0.0f, 0.0f, 1.0f);
					int segments = 32;
					glm::dvec4 color = sprite.Color;
					float angleStep = 2.0f * 3.14159265358979323846f / segments;
					for (int i = 0; i < segments; ++i)
					{
						float angle1 = i * angleStep;
						float angle2 = (i + 1) * angleStep;
						glm::dvec4 p1(
							center.x + radius * cos(angle1),
							center.y + radius * sin(angle1),
							center.z,
							1.0f
						);
						glm::dvec4 p2(
							center.x + radius * cos(angle2),
							center.y + radius * sin(angle2),
							center.z,
							1.0f
						);
						glm::dvec4 triangle[3] = { center, p1, p2 };
						//Renderer2D::DrawTriangle(transform, color, triangle);
					}
					break;
				}
				default:
					break;
				}
			}
		}

		////////////////////////////MeshRendererComponent/////////////////

		{
			auto view = m_Registry.view<TransformComponent, Mesh2DComponent,SpriteRendererComponent>();
			view.each([&](auto entity, TransformComponent& transform, Mesh2DComponent& meshCom, SpriteRendererComponent sprite)
				{
					auto& mesh = meshCom.Mesh;
					MeshRenderType meshRenderType = mesh.m_RenderType;

					uint32_t unitWidth = mesh.GetWidth();
					uint32_t unitHeight = mesh.GetHeight();
					uint32_t size = unitWidth * unitHeight;

					glm::dvec4 vertex0Postion = mesh.GetVertexPosition(0);

					glm::dvec4 transitionVector = glm::dvec4(0.0f);

					if(transform.Rotation != mesh.GetRotation())
					{
						mesh.SetRotation(transform.Rotation);
					}

					//glm::dvec3 scaleRatio = transform.Scale / mesh.GetScale();

					if(transform.Scale != mesh.GetScale())
					{
						for (int i = 0; i < size; i++)
						{
							glm::dvec4 vertexPos = mesh.GetVertexPosition(i) - vertex0Postion;
							vertexPos = glm::scale(glm::dmat4(1.0f), transform.Scale / mesh.GetScale()) * vertexPos;
							mesh.SetVertexPosition(i, vertexPos + vertex0Postion);
						}
						mesh.SetScale(transform.Scale);
					}


					switch (meshRenderType)
					{
					case MeshRenderType::Quad:
						
						vertex0Postion.w = 0.0f;

						transitionVector = glm::dvec4(transform.Position - glm::dvec3(vertex0Postion), 0.0f);



						for (int i = 0; i < unitHeight; i++)
						{
							for (int j = 0; j < unitWidth; j++)
							{
								mesh.SetVertexPosition(j, i, mesh.GetVertexPosition(j, i) + transitionVector);
							}
						}
						for (int i = 0; i < unitHeight - 1; i++)
						{
							for (int j = 0; j < unitWidth - 1; j++)
							{
								glm::dvec4 l1 = mesh.GetVertexPosition(j, i) - vertex0Postion;
								glm::dvec4 r1 = mesh.GetVertexPosition(j + 1, i) - vertex0Postion;
								glm::dvec4 r2 = mesh.GetVertexPosition(j + 1, i + 1) - vertex0Postion;
								glm::dvec4 l2 = mesh.GetVertexPosition(j, i + 1) - vertex0Postion;

								glm::dvec4 QuadVertex[4] = { l1, r1, r2, l2 };
								//Renderer2D::DrawQuad(transform, sprite.Color, QuadVertex);
							}
						}

						break;
					case MeshRenderType::Line:
						
						vertex0Postion.w = 0.0;

						transitionVector = glm::dvec4(transform.Position - glm::dvec3(vertex0Postion), 0.0);

						for (int i = 0; i < unitHeight; i++)
						{
							mesh.SetVertexPosition(i, mesh.GetVertexPosition( i) + transitionVector);
							
						}
						for (int i = 0; i < unitHeight - 1; i++)
						{
							glm::dvec4 l1 = mesh.GetVertexPosition(i) - vertex0Postion;
							glm::dvec4 l2 = mesh.GetVertexPosition(i + 1) - vertex0Postion;

							//Renderer2D::DrawLine(transform, sprite.Color, l1, l2);
						}
							//std::cout << "\n";
						break;
					default:
						break;
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
					glm::dvec3 entityVelocity =  rb.rb.GetVelocity();

					//std::cout << " Scene::OnUpdateSimulation" << "\n";
					bc.SetVertexPosition(transform.Position);

					transform.Position += bc.OnUpdate(ts, entityVelocity);
					transform.RecalculateTransform();

				});
		}


		///////////////SMS2DComponent//////
		{
			auto view = m_Registry.view<Mesh2DComponent, SMS2DComponent>();
			view.each([&](auto entity, Mesh2DComponent& mesherendercom, SMS2DComponent& smsCom)
				{
					auto& mesh = mesherendercom.Mesh;
					auto& sms = smsCom.SMS;

					//collider.SetVertexSize(render.GetHeight() * render.GetWidth());
					//uint32_t size = collider.m_VertexSize;
					//
					//if (size <= 0) { return; }
					//if (!sms)
					//{
					//	sms.SetIntialize(true);
					//	for (int i = 0; i < size; i++)
					//	{
					//		sms.AddPoint(render.GetVertexPosition(i));
					//	}
					//	uint32_t unitWidth = render.GetWidth();
					//	uint32_t unitHeight = render.GetHeight();


					//	for (int i = 0; i < unitHeight; i++)
					//	{
					//		for (int j = 0; j < unitWidth; j++)
					//		{
					//			
					//			uint32_t index = i * unitWidth + j;
					//			
					//			if (j != unitWidth - 1)sms.AddSpring(index, index + 1);
					//			else if (i != unitHeight - 1)sms.AddSpring(index, index + unitWidth);
					//			if (j != unitWidth - 1 && i != unitHeight - 1)
					//			{
					//				sms.AddSpring(index, index + unitWidth + 1);
					//				sms.AddSpring(index + 1 , index + unitWidth);
					//			
					//			}
					//		}
					//	}
					//}
					//for (int i = 0; i < size; i++)
					//{
					//	sms.SetPointPosition(i, render.GetVertexPosition(i));

					//	collider.m_VertexPosition[i] = render.m_VertexPosition[i];//TODO:改成函数
					//}


					//glm::dvec3 velocityArray[10000];
					//for (int i = 0; i < 10; i++)
					//{
					//	sms.OnUpdate(ts);
				
					//	for (int i = 0; i < collider.m_VertexSize; i++)
					//	{
					//		velocityArray[i] = sms.GetPointVelocity(i);
					//	}

					//	collider.OnUpdate(ts, velocityArray);

					//	for (int i = 0; i < size; i++)
					//	{
					//		sms.SetPointPosition(i, collider.GetVertexPosition(i));
					//	}
					//}


					//for (int i = 0; i < collider.m_VertexSize; i++)
					//{
					//	render.m_VertexPosition[i] = glm::dvec4(collider.m_VertexPosition[i], 1.0f);
					//}

				});
		}

		///////////////FEM2DComponent//////
		{
			auto view = m_Registry.view<Mesh2DComponent, FEM2DComponent>();
			view.each([&](auto entity, Mesh2DComponent& mesherendercom, FEM2DComponent& femCom)
				{
					auto& render = mesherendercom.Mesh;
					auto& fem = femCom.FEM;

					//collider.SetVertexSize(render.GetHeight() * render.GetWidth());
					//uint32_t size = collider.m_VertexSize;
					//if (size <= 0) { return; }

					//if (!fem)
					//{

					//	fem.SetInitialize(true);

					//	// 1. 添加节点
					//	for (int i = 0; i < size; i++)
					//	{
					//		glm::dvec4 pos4 = render.GetVertexPosition(i);
					//		fem.AddNode(glm::dvec2(pos4.x, pos4.y));
					//	}

					//	uint32_t unitWidth = render.GetWidth();
					//	uint32_t unitHeight = render.GetHeight();

					//	// 2. 添加三角形
					//	for (int i = 0; i < unitHeight - 1; i++)
					//	{
					//		for (int j = 0; j < unitWidth - 1; j++)
					//		{
					//			uint32_t A = i * unitWidth + j;
					//			uint32_t B = i * unitWidth + (j + 1);
					//			uint32_t C = (i + 1) * unitWidth + j;
					//			uint32_t D = (i + 1) * unitWidth + (j + 1);

					//			// 稍微降低初始刚度以防万一，或者确保子步够多
					//			fem.AddTriangle(A, B, C, 5000.0f, 0.3f);
					//			fem.AddTriangle(B, D, C, 5000.0f, 0.3f);
					//		}
					//	}

					//	// 3. 【核心修复】强制刷新参考构型
					//	// 必须确保 FEM 内部记录的“初始形状”与“当前形状”完全一致
					//	for (auto& elem : fem.m_Elements)
					//	{
					//		const FEMNode& n0 = fem.m_Nodes[elem.NodeIndices[0]];
					//		const FEMNode& n1 = fem.m_Nodes[elem.NodeIndices[1]];
					//		const FEMNode& n2 = fem.m_Nodes[elem.NodeIndices[2]];

					//		glm::dmat2 Dm(n1.Position - n0.Position, n2.Position - n0.Position);
					//		// 使用 abs 防止顶点顺序导致的负面积
					//		elem.ReferenceArea = 0.5f * std::abs(glm::determinant(Dm));

					//		if (std::abs(elem.ReferenceArea) > 1e-6f) {
					//			elem.InverseReferenceMatrix = glm::inverse(Dm);
					//		}
					//	}
					//}

					//// 4. 同步位置 (如果是从外部同步)
					//for (int i = 0; i < size; i++)
					//{
					//	glm::dvec4 pos4 = render.GetVertexPosition(i);
					//	fem.SetNodePosition(i, glm::dvec2(pos4.x, pos4.y));
					//	collider.m_VertexPosition[i] = render.m_VertexPosition[i];
					//}

					//// 5. 物理更新
					//glm::dvec3 velocityArray[10000];
					//int subSteps = 10; // 如果还爆炸，增加到 20
					//float dt = ts;
					////dt /= 10.0f;
					//for (int s = 0; s < subSteps; s++)
					//{
					//	fem.ComputeForces();
					//	fem.Integrate(dt / subSteps);

					//	for (int i = 0; i < size; i++)
					//	{
					//		glm::dvec2 femVel = fem.GetNodeVelocity(i);
					//		velocityArray[i] = glm::dvec3(femVel.x, femVel.y, 0.0f);
					//	}
					//	collider.OnUpdate(ts, velocityArray);

					//	for (int i = 0; i < size; i++)
					//	{
					//		fem.SetNodePosition(i, collider.GetVertexPosition(i));
					//	}

					//}


					//for (int i = 0; i < size; i++)
					//{
					//	render.m_VertexPosition[i] = glm::dvec4(collider.m_VertexPosition[i], 1.0f);
					//}
				});
		}
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		glm::dmat4 mainCameraProjection = glm::dmat4(1.0);
		glm::dmat4 cameraTransform  = glm::dmat4(0.0);
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

		if (mainCameraProjection != glm::dmat4(1.0))
		{
			Renderer2D::BeginScene(mainCameraProjection, cameraTransform);
		}
		//Sprite///////////
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				glm::dvec4 Triangle1[3] = {
					glm::dvec4(-0.5f, -0.5f, 0.0f, 1.0f),
					glm::dvec4(0.5f, -0.5f, 0.0f, 1.0f),
					glm::dvec4(0.5f,  0.5f, 0.0f, 1.0f)
				};
				glm::dvec4 Triangle2[3] = {
					glm::dvec4(-0.5f, -0.5f, 0.0f, 1.0f),
					glm::dvec4(-0.5f, 0.5f, 0.0f, 1.0f),
					glm::dvec4(0.5f,  0.5f, 0.0f, 1.0f)
				};
				//Renderer2D::DrawTriangle(transform, sprite.Color, Triangle1);
				//Renderer2D::DrawTriangle(transform, sprite.Color, Triangle2);

			}
		}
		////////////////////////////MeshRendererComponent/////////////////

		{
			auto view = m_Registry.view<TransformComponent, Mesh2DComponent, SpriteRendererComponent>();
			view.each([&](auto entity, TransformComponent& transform, Mesh2DComponent& meshCom, SpriteRendererComponent sprite)
				{
					auto& mesh = meshCom.Mesh;
					uint32_t unitWidth = mesh.GetWidth();
					uint32_t unitHeight = mesh.GetHeight();

					glm::dvec4 vertex0Postion = mesh.GetVertexPosition(0);

					vertex0Postion.w = 0.0;

					glm::dvec4 transitionVector = glm::dvec4(transform.Position - glm::dvec3(vertex0Postion), 0.0);

					for (int i = 0; i < unitHeight; i++)
					{
						for (int j = 0; j < unitWidth; j++)
						{
							mesh.SetVertexPosition(j, i, mesh.GetVertexPosition(j, i) + transitionVector);
						}
					}


					for (int i = 0; i < unitHeight - 1; i++)
					{
						for (int j = 0; j < unitWidth - 1; j++)
						{

							glm::dvec4 l1 = mesh.GetVertexPosition(j, i) - vertex0Postion;
							glm::dvec4 r1 = mesh.GetVertexPosition(j + 1, i) - vertex0Postion;
							glm::dvec4 r2 = mesh.GetVertexPosition(j + 1, i + 1) - vertex0Postion;
							glm::dvec4 l2 = mesh.GetVertexPosition(j, i + 1) - vertex0Postion;

							glm::dvec4 QuadVertex[4] = { l1, r1, r2, l2 };

							//Renderer2D::DrawQuad(transform, sprite.Color, QuadVertex);
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