#include "rgpch.h"
#include "Serializer.h"


#include"Resug/Scene/Components.h"


namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);  // 设置为流式风格 [x, y, z]
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;
        
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);  // 设置为流式风格 [x, y, z, w]
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace Resug
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		return out <<YAML::BeginSeq<< v.x << v.y << v.z <<YAML::EndSeq ;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		return out << YAML::BeginSeq << v.x << v.y << v.z<<v.w << YAML::EndSeq;
	}

	Serializer::Serializer(Ref<Scene> scene)
		:m_Scene(scene)
	{
	}

	void Serializer::Serialize(const std::string filepath)
	{
		std::cout << "???";
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "wuTitle";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		for (auto& e : m_Scene->GetRegistry().storage<entt::entity>())
		{
			Entity entity{ e, m_Scene.get() };

			if (!entity)
				continue;
			Serialize(out, entity);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
		
	}
	void Serializer::Serialize(YAML::Emitter& out , Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "123123";

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Tag" << YAML::Value << entity.GetComponent<TagComponent>().Tag;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<TransformComponent>())
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Position" << YAML::Value << transform.Position;
			out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.Scale;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			auto& sprite = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Color" << YAML::Value << sprite.Color;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			auto& cameraComp = entity.GetComponent<CameraComponent>();
			auto& sceneCamera = cameraComp.camera;

			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			// CameraComponent 的属性
			out << YAML::Key << "Primary" << YAML::Value << cameraComp.Primary;
			out << YAML::Key << "Fixed" << YAML::Value << cameraComp.Fixed;

			// SceneCamera 的属性
			out << YAML::Key << "CameraType" << YAML::Value << (int)sceneCamera.GetCameraType();
			out << YAML::Key << "CameraSize" << YAML::Value << sceneCamera.GetCameraSize();
			out << YAML::Key << "CameraNear" << YAML::Value << sceneCamera.GetCameraNear();
			out << YAML::Key << "CameraFar" << YAML::Value << sceneCamera.GetCameraFar();
			//out << YAML::Key << "CameraProjection" << YAML::Value << sceneCamera.GetCameraFar();

			out << YAML::EndMap;
		}


		out << YAML::EndMap;
	}
	bool Serializer::Derialize(const std::string filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entityNode : entities)
			{
				// 获取实体名称
				std::string entityName = "Unnamed Entity";
				if (entityNode["TagComponent"] )
					entityName = entityNode["TagComponent"]["Tag"].as<std::string>();

				// 创建实体
				Entity entity = m_Scene->CreateEntity(entityName);

				// TransformComponent
				if (entityNode["TransformComponent"])
				{
					auto& transform = entity.GetComponent<TransformComponent>();
					auto tfNode = entityNode["TransformComponent"];

					if (tfNode["Position"])
						transform.Position = tfNode["Position"].as<glm::vec3>();
					if (tfNode["Rotation"])
						transform.Rotation = tfNode["Rotation"].as<glm::vec3>();
					if (tfNode["Scale"])
						transform.Scale = tfNode["Scale"].as<glm::vec3>();

					transform.RecalculateTransform();
				}

				// CameraComponent
				if (entityNode["CameraComponent"])
				{
					auto& cameraComp = entity.AddComponent<CameraComponent>();
					auto camNode = entityNode["CameraComponent"];

					if (camNode["Primary"])
						cameraComp.Primary = camNode["Primary"].as<bool>();
					if (camNode["Fixed"])
						cameraComp.Fixed = camNode["Fixed"].as<bool>();
					if (camNode["CameraType"])
						cameraComp.camera.SetCameraType((SceneCamera::CameraType)camNode["CameraType"].as<int>());
					if (camNode["CameraSize"])
						cameraComp.camera.SetCameraSize(camNode["CameraSize"].as<float>());
					if (camNode["CameraNear"])
						cameraComp.camera.SetCameraNear(camNode["CameraNear"].as<float>());
					if (camNode["CameraFar"])
						cameraComp.camera.SetCameraFar(camNode["CameraFar"].as<float>());
				}

				// SpriteRendererComponent
				if (entityNode["SpriteRendererComponent"] )
				{
					auto& sprite = entity.AddComponent<SpriteRendererComponent>();
					sprite.Color = entityNode["SpriteRendererComponent"]["Color"].as<glm::vec4>();
				}

			}//for (auto entityNode : entities)

		}
		return true;
	}
}