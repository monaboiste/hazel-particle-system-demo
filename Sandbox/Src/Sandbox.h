#pragma once

#include "ParticleSystem.h"

class Sandbox : public Hazel::Layer
{
public:
	Sandbox();
	virtual ~Sandbox() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hazel::Event& e) override;
private:
	Hazel::OrthographicCameraController m_cameraController;

	ParticleProps m_particle;
	ParticleSystem m_particleSystem;

};