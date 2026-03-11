#pragma once

#include"Entity.h"

namespace Resug
{
	class ScriptableEntity
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	protected:
		Entity m_Entity;
		friend class Scene;
	};
}