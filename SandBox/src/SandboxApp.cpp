#include<Resug.h>



class ExampleLayer : public Resug::Layer
{
public:
	ExampleLayer() :Layer("Example")
	{
	}

	virtual void OnUpdate()override
	{
		//RG_CLIENT_INFO("ExampleLayer::Update");
	}
	virtual void OnEvent(Resug::Event& e) override
	{
		RG_CLIENT_TRACE("{0}", e.ToString());
	}

private:

};


class Sandbox : public Resug::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox(){}

private:

};


Resug::Application* Resug::CreateApplication()
{
	return new Sandbox();
}