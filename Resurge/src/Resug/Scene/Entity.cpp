#include "rgpch.h"

#include "Entity.h"
namespace Resug
{
	Entity::Entity(entt::entity handle, Scene* scene)
		:m_Entity(handle), m_Scene(scene)
	{

	}
}
