#include "Player.h"

Player::Player(const Config& config)
     : startPosition(config.Position), startVelocity(config.Velocity), velocity(config.Velocity), enginePower(config.EnginePower)
{
    quad.Texture = config.Texture;
    quad.Position = config.Position;
    quad.Size = { config.Width, config.Height };
}

glm::vec3 Player::GetPosition() const
{
    return quad.Position;
}

void Player::OnUpdate(st::Input* input, st::Timestep timestep, float gravity)
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
}

void Player::OnRender(st::Renderer* renderer)
{
    renderer->SubmitQuad(quad);
}

void Player::Reset()
{
    quad.Position = startPosition;
    quad.RotationInDegrees = 0;
    velocity = startVelocity;
}
