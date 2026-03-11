#pragma once

#include"entt.hpp"

#include"Resug/Core/Timestep.h"



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

		void OnUpdate(Timestep ts);

		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth, m_ViewportHeight;

		friend class Entity;
	};
}
