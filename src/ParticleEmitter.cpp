#include "ParticleEmitter.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

ParticleEmitter::ParticleEmitter()
{
    particles.resize(1000);
}

void ParticleEmitter::Emit(const Config& config)
{
    Particle& particle = particles[particleIndex];

    particle.Active = true;
    particle.Position = config.Position;
    particle.Rotation = random.Next() * 2.0f * glm::pi<float>();

    particle.Velocity = config.Velocity;
    particle.Velocity.x += config.VelocityVariation.x * (random.Next(0, 2) - 2.0f);
    particle.Velocity.y += config.VelocityVariation.y * (random.Next(0, 2) - 2.0f);

    particle.ColorBegin = config.ColorBegin;
    particle.ColorEnd = config.ColorEnd;

    particle.SizeBegin = config.SizeBegin + config.SizeVariation * (random.Next(0, 5) - 5.0f);
    particle.SizeEnd = config.SizeEnd;

    particle.LifeTime = config.LifeTime;
    particle.LifeRemaining = config.LifeTime;

    particleIndex--;
    if (particleIndex < 0)
    {
        particleIndex = particles.size() - 1;
    }
}

void ParticleEmitter::OnUpdate(st::Timestep timestep)
{
    for (auto& particle : particles)
    {
        if (!particle.Active)
        {
            continue;
        }
        if (particle.LifeRemaining <= 0.0f)
        {
            particle.Active = false;
            continue;
        }
        particle.LifeRemaining -= timestep;
        particle.Position += particle.Velocity * (float) timestep;
        particle.Rotation += 10.0f * timestep;
    }
}

void ParticleEmitter::OnRender(st::Renderer* renderer)
{
    for (auto& particle : particles)
    {
        if (!particle.Active)
        {
            continue;
        }

        float life = particle.LifeRemaining / particle.LifeTime;
        glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
        color.a = color.a * life;

        float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

        st::Quad quad;
        quad.Position = { particle.Position.x, particle.Position.y, 0.9f };
        quad.Size = { size, size };
        quad.Color = color;
        quad.RotationInDegrees = particle.Rotation;
        renderer->SubmitQuad(quad);
    }
}

void ParticleEmitter::Reset()
{
    for (auto& particle : particles)
    {
        particle.Active = false;
    }
}
