#include "Player.h"

constexpr float Player::MAX_VELOCITY = 30.0f;

Player::Player(const Config& config)
        : startPosition(config.Position), startVelocity(config.Velocity), velocity(config.Velocity), enginePower(config.EnginePower)
{
    quad.Texture = config.Texture;
    quad.Position = config.Position;
    quad.Size = { config.Width, config.Height };

    smokeParticle.Position = { 0.0f, 0.0f };
    smokeParticle.Velocity = { -2.0f, 0.0f };
    smokeParticle.VelocityVariation = { 0.5f, 0.1f };
    smokeParticle.SizeBegin = 1.0f;
    smokeParticle.SizeEnd = 0.0f;
    smokeParticle.SizeVariation = 0.5f;
    smokeParticle.ColorBegin = { 0.8f, 0.8f, 0.8f, 1.0f };
    smokeParticle.ColorEnd = { 0.6f, 0.6f, 0.6f, 1.0f };
    smokeParticle.LifeTime = 4.0f;

    flameParticle.Position = { 0.0f, 0.0f };
    flameParticle.Velocity = { -2.0f, 0.0f };
    flameParticle.VelocityVariation = { 1.0f, 0.25f };
    flameParticle.SizeBegin = 1.0f;
    flameParticle.SizeEnd = 0.0f;
    flameParticle.SizeVariation = 0.5f;
    flameParticle.ColorBegin = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
    flameParticle.ColorEnd = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
    flameParticle.LifeTime = 1.0f;

    time = 0.0f;
    smokeEmitInterval = 0.4f;
    smokeNextEmitTime = smokeEmitInterval;
}

glm::vec3 Player::GetPosition() const
{
    return quad.Position;
}

glm::vec2 Player::GetSize() const
{
    return quad.Size;
}

void Player::OnUpdate(st::Input* input, st::Timestep timestep, float gravity)
{
    time += timestep;
    if (input->IsKeyPressed(st::KeyCode::KEY_SPACE))
    {
        velocity.y += enginePower;
        if (velocity.y < 0.0f)
        {
            velocity.y += enginePower * 0.5f;
        }
        flameParticle.Position = quad.Position + getParticleEmissionOffset();
        flameParticle.Velocity.y = -velocity.y * 0.1f - 0.1f;
        particleEmitter.Emit(flameParticle);
    }
    else
    {
        velocity.y -= gravity;
    }
    velocity.y = glm::clamp(velocity.y, -MAX_VELOCITY, MAX_VELOCITY);

    //velocity.y = 0.0f;

    quad.Position += velocity * (float) timestep;
    quad.RotationInDegrees = GetRotation();

    if (time > smokeNextEmitTime)
    {
        smokeParticle.Position = quad.Position + getParticleEmissionOffset();
        particleEmitter.Emit(smokeParticle);
        smokeNextEmitTime += smokeEmitInterval;
    }
    particleEmitter.OnUpdate(timestep);
}

float Player::GetRotation() const
{
    return velocity.y * 2.0f - 90.0f;
}

void Player::OnRender(st::Renderer* renderer)
{
    particleEmitter.OnRender(renderer);
    renderer->SubmitQuad(quad);
}

void Player::Reset()
{
    quad.Position = startPosition;
    quad.RotationInDegrees = 0;
    velocity = startVelocity;
    particleEmitter.Reset();
}

void Player::FillTransformedVertices(glm::vec4* vertices) const
{
    vertices[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    vertices[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
    vertices[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
    vertices[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
    for (int i = 0; i < 4; i++)
    {
        const glm::mat4& translation = glm::translate(glm::mat4(1.0f), { quad.Position.x, quad.Position.y, 0.0f });
        const glm::mat4& rotation = glm::rotate(glm::mat4(1.0f), glm::radians(quad.RotationInDegrees), { 0.0f, 0.0f, 1.0f });
        const glm::mat4& scale = glm::scale(glm::mat4(1.0f), { quad.Size.x, quad.Size.y, 1.0f });
        vertices[i] = translation * rotation * scale * vertices[i];
    }
}

glm::vec3 Player::getParticleEmissionOffset() const
{
    const auto& emissionPoint = glm::vec4(0.0f, -(quad.Size.y / 2), 0.0f, 1.0f);
    glm::vec4 rotatedEmissionPoint = glm::rotate(glm::mat4(1.0f), glm::radians(GetRotation()), { 0.0f, 0.0f, 1.0f }) * emissionPoint;
    return glm::vec3{ rotatedEmissionPoint.x, rotatedEmissionPoint.y, 1.0f };
}
