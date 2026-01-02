#include<Resug.h>


class Sandbox : public Resug::Application
{
public:
	Sandbox(){}
	~Sandbox(){}

private:

};


Resug::Application* Resug::CreateApplication()
{
	return new Sandbox();
}