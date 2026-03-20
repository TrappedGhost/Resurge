#pragma once

#include"entt.hpp"

#include"Resug/Core/Timestep.h"

#include"Resug/Renderer/EditerCamera.h"

namespace Resug
{
	class Entity;
	
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(std::string name = "Entity");

		void DestroyEntity(Entity entity);

		entt::registry& GetRegistry() { return m_Registry; }

		Entity GetPrimaryCameraEntity();

		void OnUpdateEditor(Timestep ts);
		void OnUpdateSimulation(Timestep ts);
		void OnUpdateRuntime(Timestep ts);

		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		entt::registry m_Registry;

		EditerCamera m_editerCamera;

		uint32_t m_ViewportWidth, m_ViewportHeight;

		friend class Entity;
	};
}
