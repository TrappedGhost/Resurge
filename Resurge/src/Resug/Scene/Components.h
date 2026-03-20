#pragma once
#include<glm/glm.hpp>

#include<glm/gtc/matrix_transform.hpp>

#include"Resug/Core/Timestep.h"
#include"Resug/Renderer/SceneCamera.h"

#include"ScriptableEntity.h"

#include"Resug/Simulation/SpringMassSystem.h"
#include"Resug/Simulation/FEM.h"
#include"Resug/Simulation/RigidBody.h"
#include"Resug/Simulation/Collider.h"

#include<iostream>

namespace Resug
{
	struct  TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string tag)
			:Tag(tag) {
		}

		
	};

	struct  TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		glm::vec3 Position{ 0.0f,0.0f,0.0f };
		glm::vec3 Rotation{ 0.0f,0.0f,0.0f };
		glm::vec3 Scale{ 1.0f,1.0f,1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			:Transform(transform){ }
		glm::mat4 GetTransform()
		{
			RecalculateTransform();
			return Transform;
		}
		void RecalculateTransform()
		{
			glm::mat4 RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), { 1,0,0 })
				* glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), { 0,1,0 })
				* glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), { 0,0,1 });

			Transform = glm::translate(glm::mat4(1.0f), Position)
				* RotationMat
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }

	};

	struct SpriteRendererComponent
	{
		//SpringMassSystem SMSystem;
		//FiniteElementMesh2D FEMSystem;
		//glm::vec4 vertexPosition[4];//TODO : 移动到物理组件。


		glm::vec4 Color{ 1.0f,1.0f,1.0f,1.0f };
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(glm::vec4 color) 
			:Color(color)
		{
			
		}

	};

	struct CameraComponent
	{
		SceneCamera Camera ;
		bool Primary = true;
		bool Fixed = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(glm::mat4 projection)
			:Camera({projection}){ }
	};

	struct NativeScriptComponent
	{
		ScriptableEntity*  Instance = nullptr;

		std::function<void()> InstanceFunction;
		std::function<void()> DestroyInstanceFunction;

		std::function<void(ScriptableEntity*)> OnCreateFunction;
		std::function<void(ScriptableEntity*)> OnDeatroyFunction;
		std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunction;

		template<typename T>
		void Bind()
		{
			InstanceFunction = [&]() {Instance = new T(); };
			DestroyInstanceFunction = [&]() {delete (T*)Instance; Instance = nullptr; };

			OnCreateFunction = [](ScriptableEntity* instance) {((T*)instance)->OnCreate(); };
			OnDeatroyFunction = [](ScriptableEntity* instance) {((T*)instance)->OnDestroy(); };
			OnUpdateFunction = [](ScriptableEntity* instance, Timestep ts) {((T*)instance)->OnUpdate(ts); };
		}
	};


	struct RigidBodyComponent
	{
		RigidBody rb;

		RigidBodyComponent() = default;
		RigidBodyComponent(const RigidBodyComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		BoxCollider2D BoxCollider;

		glm::vec3  RelativePosition[4];


		void SetVertexPosition(glm::vec3 Position)
		{
			RelativePosition[0] = glm::vec3(-0.5f, -0.5f, 0.0f);
			RelativePosition[1] = glm::vec3( 0.5f, -0.5f, 0.0f);
			RelativePosition[2] = glm::vec3( 0.5f,  0.5f, 0.0f);
			RelativePosition[3] = glm::vec3(-0.5f,  0.5f, 0.0f);
			for (int i = 0; i < 4; i++)
			{
				BoxCollider.m_VertexPosition[i] = Position + RelativePosition[i];
			}
		}
		glm::vec3 OnUpdate(float ts, glm::vec3 velocity)
		{
			return BoxCollider.OnUpdate(ts, velocity);
		}
		bool CheckOnGround()
		{
			return BoxCollider.m_OnGround;
		}

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;

	};

}