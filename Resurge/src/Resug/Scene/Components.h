#pragma once
#include<glm/glm.hpp>

#include<glm/gtc/matrix_transform.hpp>

#include"Resug/Core/Timestep.h"
#include"Resug/Renderer/SceneCamera.h"
#include"Resug/Renderer/Mesh2D.h"

#include"ScriptableEntity.h"

#include"Resug/Simulation/Geometry2D.h"
#include"Resug/Simulation/SpringMassSystem.h"
#include"Resug/Simulation/FiniteElementMesh2D.h"
#include"Resug/Simulation/RigidBody.h"
#include"Resug/Simulation/BoxCollider2D.h"

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
		glm::dmat4 Transform{ 1.0f };

		glm::dvec3 Position{ 0.0f,0.0f,0.0f };
		glm::dvec3 Rotation{ 0.0f,0.0f,0.0f };
		glm::dvec3 Scale{ 1.0f,1.0f,1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::dmat4& transform)
			:Transform(transform){ }
		glm::dmat4 GetTransform()
		{
			RecalculateTransform();
			return Transform;
		}
		void RecalculateTransform()
		{
			glm::dmat4 RotationMat = glm::rotate(glm::dmat4(1.0f), glm::radians(Rotation.x), { 1,0,0 })
				* glm::rotate(glm::dmat4(1.0f), glm::radians(Rotation.y), { 0,1,0 })
				* glm::rotate(glm::dmat4(1.0f), glm::radians(Rotation.z), { 0,0,1 });

			Transform = glm::translate(glm::dmat4(1.0f), Position)
				* RotationMat
				* glm::scale(glm::dmat4(1.0f), Scale);
		}

		operator glm::dmat4& () { return Transform; }
		operator const glm::dmat4& () const { return Transform; }

	};

	struct SpriteRendererComponent
	{

		enum class SpriteTpye
		{
			Quad = 0,
			Circle = 1,
			Mesh = 2
		};

		glm::vec4 Color{ 1.0f,1.0f,1.0f,1.0f };

		// Circle
		float radius = 0.5f;

		SpriteTpye Type  = SpriteTpye::Quad;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(glm::vec4 color, SpriteTpye type) 
			:Color(color), Type(type)
		{
			
		}

	};


	struct Mesh2DComponent
	{

		Mesh2D Mesh;

		Mesh2DComponent() = default;
		Mesh2DComponent(const Mesh2DComponent&) = default;
		Mesh2DComponent( uint32_t width, uint32_t height, Mesh2DType type)
			: Mesh( width, height, type)
		{
		
		}
		Mesh2DComponent( uint32_t width, uint32_t height, Mesh2DType type, MeshRenderType renderType)
			: Mesh( width, height, type, renderType)
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




	////////////////////////////////////////////Simulation/////////////////////////////


	struct RigidBodyComponent
	{
		RigidBody rb;

		RigidBodyComponent() = default;
		RigidBodyComponent(const RigidBodyComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		BoxCollider2D BoxCollider;

		glm::dvec3  RelativePosition[4];


		void SetVertexPosition(glm::dvec3 Position)
		{
			RelativePosition[0] = glm::dvec3(-0.5f, -0.5f, 0.0f);
			RelativePosition[1] = glm::dvec3( 0.5f, -0.5f, 0.0f);
			RelativePosition[2] = glm::dvec3( 0.5f,  0.5f, 0.0f);
			RelativePosition[3] = glm::dvec3(-0.5f,  0.5f, 0.0f);
			for (int i = 0; i < 4; i++)
			{
				BoxCollider.m_VertexPosition[i] = Position + RelativePosition[i];
				BoxCollider.m_Polygon->vertiesPosition[i] = BoxCollider.m_VertexPosition[i];
			}
		}
		glm::dvec3 OnUpdate(float ts, glm::dvec3 velocity)
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

	//struct MeshCollider2DComponent
	//{
	//	MeshCollider2D MeshCollider;

	//	glm::dvec3  RelativePosition[4];


	//	void SetVertexPosition(glm::dvec3 Position)
	//	{
	//		RelativePosition[0] = glm::dvec3(-0.5f, -0.5f, 0.0f);
	//		RelativePosition[1] = glm::dvec3(0.5f, -0.5f, 0.0f);
	//		RelativePosition[2] = glm::dvec3(0.5f, 0.5f, 0.0f);
	//		RelativePosition[3] = glm::dvec3(-0.5f, 0.5f, 0.0f);
	//		for (int i = 0; i < 4; i++)
	//		{
	//			MeshCollider.m_VertexPosition[i] = Position + RelativePosition[i];
	//		}
	//	}
	//	glm::dvec3 OnUpdate(float ts, glm::dvec3* velocity)
	//	{
	//		return MeshCollider.OnUpdate(ts, velocity);
	//	}

	//	MeshCollider2DComponent() = default;
	//	MeshCollider2DComponent(const MeshCollider2DComponent&) = default;

	//};

	struct SMS2DComponent
	{
		SpringMassSystem SMS;


		SMS2DComponent() = default;
		SMS2DComponent(const SMS2DComponent&) = default;
	};



	struct FEM2DComponent
	{
		FEMSystem2D FEM;


		FEM2DComponent() = default;
		FEM2DComponent(const FEM2DComponent&) = default;
	};


	////////////////////////////////////////////NativeScriptComponent/////////////////////////////

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

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

}