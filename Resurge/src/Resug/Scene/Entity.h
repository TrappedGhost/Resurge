#pragma once

#include"Scene.h"
#include"Resug/Core/Core.h"
namespace Resug
{
	class Scene;

	class Entity
	{
	public:
		Entity() = default;
		Entity(const Entity& other) = default;
		Entity(entt::entity handle, Scene* scene);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			RG_CORE_ASSERT(!HasComponent<T>(), "entity has have the component");
			return m_Scene->GetRegistry().emplace<T>(m_Entity, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			RG_CORE_ASSERT(HasComponent<T>(), "entity is not have the component");
			return m_Scene->GetRegistry().get<T>(m_Entity);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->GetRegistry().all_of<T>(m_Entity);
		}

		template<typename T>
		bool RemoveComponent()
		{
			return m_Scene->GetRegistry().remove<T>(m_Entity);
		}

		
		operator bool() const { return m_Entity != entt::null; }
		operator entt::entity() const{ return m_Entity; }
		operator uint32_t() const{ return uint32_t(m_Entity); }

		bool operator == (const Entity& other)const { return m_Entity == other.m_Entity; }

	private:
		entt::entity m_Entity{ entt::null };
		Scene* m_Scene;
	};
}
