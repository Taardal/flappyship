#pragma once

#include <storytime/Storytime.h>

class Player
{
private:
    st::Quad quad{};
    glm::vec3 velocity;
    float enginePower;

public:
    void SetTexture(st::Ref<st::Texture> texture);

    [[nodiscard]] glm::vec3 GetPosition() const;

    void SetPosition(glm::vec3 position);

    void SetSize(glm::vec2 size);

    [[nodiscard]] glm::vec3 GetVelocity() const;

    void SetVelocity(glm::vec3 velocity);

    void SetRotationInDegrees(int32_t rotationInDegrees);

    void SetEnginePower(float enginePower);

    void OnUpdate(st::Renderer* renderer, st::Input* input, st::Timestep timestep, float gravity);

    void OnRender(st::Renderer* renderer);
};


