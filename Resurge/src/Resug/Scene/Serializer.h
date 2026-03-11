#pragma once

#include"yaml-cpp/yaml.h"

#include"Resug/Scene/Scene.h"
#include"Resug/Scene/Entity.h"
namespace Resug
{
	class Serializer
	{
	public:
		Serializer() = default;
		Serializer(Ref<Scene> scene);

		void Serialize(const std::string filepath);
		void Serialize(YAML::Emitter& out, Entity entity);

		bool Derialize(const std::string filepath);

	private:
		Ref<Scene> m_Scene;
	};
}
