#pragma once

#include <storytime/Storytime.h>

class ParticleEmitter
{
public:
    struct Config
    {
        glm::vec2 Position;
        glm::vec2 Velocity;
        glm::vec2 VelocityVariation;
        glm::vec4 ColorBegin;
        glm::vec4 ColorEnd;
        float SizeBegin;
        float SizeEnd;
        float SizeVariation;
        float LifeTime = 1.0f;
    };

private:
    struct Particle
    {
        glm::vec2 Position;
        glm::vec2 Velocity;
        glm::vec4 ColorBegin;
        glm::vec4 ColorEnd;
        float SizeBegin;
        float SizeEnd;
        float Rotation = 0.0f;
        float LifeTime = 1.0f;
        float LifeRemaining = 0.0f;
        bool Active = false;
    };
    st::Random random;
    std::vector<Particle> particles;
    int32_t particleIndex = 999;

public:
    ParticleEmitter();

    void Emit(const Config& config);

    void OnUpdate(st::Timestep timestep);

    void OnRender(st::Renderer* renderer);

    void Reset();
};