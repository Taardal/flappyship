#include "Player.h"

#include <utility>

void Player::SetTexture(st::Ref<st::Texture> texture)
{
    quad.Texture = std::move(texture);
}

glm::vec3 Player::GetPosition() const
{
    return quad.Position;
}

void Player::SetPosition(glm::vec3 position)
{
    quad.Position = position;
}

void Player::SetSize(glm::vec2 size)
{
    quad.Size = size;
}

glm::vec3 Player::GetVelocity() const
{
    return velocity;
}

void Player::SetVelocity(glm::vec3 velocity)
{
    this->velocity = velocity;
}

void Player::SetEnginePower(float enginePower)
{
    this->enginePower = enginePower;
}

void Player::OnUpdate(st::Renderer* renderer, st::Input* input, st::Timestep timestep, float gravity)
{
    if (input->IsKeyPressed(st::KeyCode::KEY_SPACE))
    {
        velocity.y += enginePower;
        if (velocity.y < 0.0f)
        {
            velocity.y += enginePower * 0.05f;
        }

        /*
        glm::vec2 emissionPoint = { 0.0f, -0.6f };
        float rotation = glm::radians(GetRotation());
        glm::vec4 rotated = glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::vec4(emissionPoint, 0.0f, 1.0f);
        m_EngineParticle.Position = m_Position + glm::vec2{ rotated.x, rotated.y };
        m_EngineParticle.Velocity.y = -m_Velocity.y * 0.2f - 0.2f;
        m_ParticleSystem.Emit(m_EngineParticle);
        */
    }
    else
    {
        velocity.y -= gravity;
    }
    velocity.y = glm::clamp(velocity.y, -20.0f, 20.0f);

    quad.Position += velocity * (float) timestep;
    //quad.Position.y = glm::clamp(quad.Position.y, -1.5f, 1.5f);

    quad.RotationInDegrees = velocity.y * 4.0f - 90.0f;

    renderer->SubmitQuad(quad);
}
