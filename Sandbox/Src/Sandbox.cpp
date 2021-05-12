#include "Hazel.h"

#include "imgui/imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include "Hazel/Core/MouseButtonCodes.h"

#include "Sandbox.h"


Sandbox::Sandbox()
	: Layer("Particle System Demo")
	, m_cameraController(1280.0f / 720.0f, true)
{
}

void Sandbox::OnAttach()
{
	m_particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_particle.SizeBegin = 0.5f, m_particle.SizeVariation = 0.3f, m_particle.SizeEnd = 0.0f;
	m_particle.LifeTime = 1.0f;
	m_particle.Velocity = { 0.0f, 0.0f };
	m_particle.VelocityVariation = { 3.0f, 1.0f };
	m_particle.Position = { 0.0f, 0.0f };

	m_cameraController.SetZoomLevel(5.0f);
}

void Sandbox::OnDetach()
{
}

void Sandbox::OnUpdate(Hazel::Timestep ts)
{
	m_cameraController.OnUpdate(ts);

	Hazel::RenderCommand::SetClearColor({ 0.15f, 0.2f, 0.2f, 1 });
	Hazel::RenderCommand::Clear();
	Hazel::Renderer2D::ResetStats();

	if (Hazel::Input::IsMouseButtonPressed(Hazel::Mouse::ButtonLeft))
	{
		auto [x, y] = Hazel::Input::GetMousePosition();
		auto width = Hazel::Application::Get().GetWindow().GetWidth();
		auto height = Hazel::Application::Get().GetWindow().GetHeight();

		auto bounds = m_cameraController.GetBounds();
		auto pos = m_cameraController.GetCamera().GetPosition();
		auto angle = glm::radians(m_cameraController.GetCamera().GetRotation());

		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();

		// translate position, pivot point is camera position
		float particleX = x * glm::cos(angle) - y * glm::sin(angle);
		float particleY = x * glm::sin(angle) + y * glm::cos(angle);

		particleX += pos.x;
		particleY += pos.y;

		m_particle.Position = { particleX,  particleY };

		for (int i = 0; i < 5; i++)
			m_particleSystem.Emit(m_particle);
	}

	m_particleSystem.OnUpdate(ts);
	m_particleSystem.OnRender(m_cameraController.GetCamera());

	Hazel::Renderer2D::EndScene();
}

void Sandbox::OnImGuiRender()
{
	ImGui::Begin("Settings");
	{
		ImGui::Text("Rotate:  Q     E");
		ImGui::Text("Camera      W");
		ImGui::Text("Move:   A   S   D");
		
		ImGui::ColorEdit4("Color Begin", glm::value_ptr(m_particle.ColorBegin));
		ImGui::ColorEdit4("Color End", glm::value_ptr(m_particle.ColorEnd));

		auto stats = Hazel::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls:  %d", stats.DrawCalls);
		ImGui::Text("Quad Count:  %d", stats.QuadCount);
		ImGui::Text("Vertices:    %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices:     %d", stats.GetTotalIndexCount());
	}
	ImGui::End();
}

void Sandbox::OnEvent(Hazel::Event& e)
{
	m_cameraController.OnEvent(e);
}
