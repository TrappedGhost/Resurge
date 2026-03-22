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

					for (int i = 0; i < unitHeight-1; i++)
					{
						for (int j = 0; j < unitWidth-1; j++)
						{
							//glm::vec4 average = mesh.CalculateAveragePosition();
							//transform.Position = glm::vec3(average.x, average.y, average.z);
							//transform.RecalculateTransform();
							mesh.CalculateRelative(glm::vec3(0.0f));

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
					glm::vec3 velocityArray[1000];
					for (int i = 0; i < collider.m_VertexSize; i++)
					{
						velocityArray[i] = glm::vec3(0.0f, 0.0f, 0.0f);
						collider.m_VertexPosition[i] = render.m_VertexPosition[i];
					}

					//collider.OnUpdate(ts, velocityArray);

					for (int i = 0; i < collider.m_VertexSize; i++)
					{
						render.m_VertexPosition[i] = glm::vec4(collider.m_VertexPosition[i],1.0f);
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

							glm::vec4 l1 = mesh.GetRelativePosition(j, i);
							glm::vec4 r1 = mesh.GetRelativePosition(j + 1, i);
							glm::vec4 r2 = mesh.GetRelativePosition(j + 1, i + 1);
							glm::vec4 l2 = mesh.GetRelativePosition(j, i + 1);
							glm::vec4 QuadVertex[4] = { l1, r1, r2, l2 };
							Renderer2D::DrawQuad(transform, mesh.GetColor(), QuadVertex);
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