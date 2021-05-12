#include "Hazel.h"
#include "Hazel\Core\EntryPoint.h"

#include "Sandbox.h"

class DemoApp : public Hazel::Application
{
public:
	DemoApp()
	{
		PushLayer(new Sandbox());
	}

	~DemoApp() = default;
};

Hazel::Application* Hazel::CreateApplication()
{
	return new DemoApp();
}