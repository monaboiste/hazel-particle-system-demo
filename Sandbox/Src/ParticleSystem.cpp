#include "ParticleSystem.h"

#include "glm/gtc/constants.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/compatibility.hpp"

#include <random>

class Random
{
public:
	static void Init()
	{
		s_randomDevice.seed(std::random_device()());
	}

	static float Float()
	{
		return (float)s_distribution(s_randomDevice) / (float)std::numeric_limits<uint32_t>::max();
	}

private:
	static std::mt19937 s_randomDevice;
	static std::uniform_int_distribution<std::mt19937::result_type> s_distribution;
};

std::mt19937 Random::s_randomDevice;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_distribution;



ParticleSystem::ParticleSystem(uint32_t maxParticles)
	: m_poolIndex(maxParticles - 1)
{
	m_particlePool.resize(maxParticles);
}

void ParticleSystem::OnUpdate(Hazel::Timestep ts)
{
	for (auto& particle : m_particlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= ts;
		particle.Position += particle.Velocity * (float)ts;
		particle.Rotation += 0.01f * ts;
	}
}

void ParticleSystem::OnRender(Hazel::OrthographicCamera& camera)
{
	Hazel::Renderer2D::BeginScene(camera);
	for (auto& particle : m_particlePool)
	{
		if (!particle.Active)
			continue;

		// Fade away particles
		float life = particle.LifeRemaining / particle.LifeTime;
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
		//color.a = color.a * life;

		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

		// Render
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.Position.x, particle.Position.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });

		glm::vec3 position(particle.Position.x, particle.Position.y, 0.0f);

		Hazel::Renderer2D::DrawRotatedQuad(position, { size, size }, particle.Rotation, color);
	}
	Hazel::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_particlePool[m_poolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

	// Color
	particle.ColorBegin = particleProps.ColorBegin;
	particle.ColorEnd = particleProps.ColorEnd;

	particle.LifeTime = particleProps.LifeTime;
	particle.LifeRemaining = particleProps.LifeTime;
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	m_poolIndex = --m_poolIndex % m_particlePool.size();
}
