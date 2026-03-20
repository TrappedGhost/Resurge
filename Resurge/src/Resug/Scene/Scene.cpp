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
		
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform, sprite.Color);

			}
		}

		{

			auto view = m_Registry.view<NativeScriptComponent>();
			view.each([&](auto entity, NativeScriptComponent& ns)
				{
					if (ns.Instance == nullptr)
					{
						ns.InstanceFunction();
						ns.Instance->m_Entity = Entity{ entity ,this };

						ns.OnCreateFunction(ns.Instance);
					}

					ns.OnUpdateFunction(ns.Instance, ts);
				});
		}

		Renderer2D::EndScene();

	}

	void Scene::OnUpdateSimulation(Timestep ts)
	{
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
				
				Renderer2D::DrawQuad(transform, sprite.Color);

			}
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