#include<Resug.h>

#include"Resug/Core/EntryPoint.h"

#include"imgui.h"

#include"Platform/OpenGL/OpenGLShader.h"
#include"glm/gtc/type_ptr.hpp"

#include"EditorLayer.h"


namespace Resug
{

	class EditorApp : public Resug::Application
	{
	public:
		EditorApp()
		{
			PushLayer(new EditorLayer());
		}
		~EditorApp() {}

	private:

	};


	Resug::Application* Resug::CreateApplication()
	{
		return new EditorApp();
	}
}