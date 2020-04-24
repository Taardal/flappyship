#pragma once

#include <storytime/Storytime.h>

class Player
{
public:
    struct Config
    {
        st::Ref<st::Texture> Texture;
        glm::vec3 Position;
        glm::vec3 Velocity;
        float Width;
        float Height;
        float EnginePower;
    };

private:
    static const float MAX_VELOCITY;

    glm::vec3 startPosition;
    glm::vec3 startVelocity;
    glm::vec3 velocity;
    float enginePower;
    st::Quad quad;

public:
    explicit Player(const Config& config);

    [[nodiscard]] glm::vec3 GetPosition() const;

    [[nodiscard]] glm::vec2 GetSize() const;

    void OnUpdate(st::Input* input, st::Timestep timestep, float gravity);

    void OnRender(st::Renderer* renderer);

    void Reset();

    void FillTransformedVertices(glm::vec4* vertices) const;
};


