#include "Level.h"

constexpr float Level::GRAVITY = 0.05f;

Level::Level(st::ResourceLoader* resourceLoader, float width, float height)
{
    backgroundQuad.Color = { 0.3f, 0.3f, 0.3f, 1.0f };
    backgroundQuad.Size = { width, height };
    backgroundQuad.Position = { 0.0f, 0.0f, 0.0f };

    ceilingQuad.Color = { 0.2f, 0.3f, 0.8f, 1.0f };
    ceilingQuad.Size = { backgroundQuad.Size.x, 5.0f };
    ceilingQuad.Position = { backgroundQuad.Position.x, backgroundQuad.Position.y + (backgroundQuad.Size.y / 2), backgroundQuad.Position.z + 0.1f };

    floorQuad.Color = ceilingQuad.Color;
    floorQuad.Size = { backgroundQuad.Size.x, ceilingQuad.Size.y };
    floorQuad.Position = { backgroundQuad.Position.x, backgroundQuad.Position.y - (backgroundQuad.Size.y / 2), ceilingQuad.Position.z };

    SpikePool::Config spikeConfig = {};
    spikeConfig.Texture = resourceLoader->LoadTexture("triangle.png");
    spikeConfig.Width = 30.0f;
    spikeConfig.WidthFactor = 0.9f;
    spikeConfig.Height = height;
    spikeConfig.ZPosition = ceilingQuad.Position.z + 0.1f;
    spikePool = new SpikePool(spikeConfig);

    Player::Config playerConfig = {};
    playerConfig.Texture = resourceLoader->LoadTexture("ship.png");
    playerConfig.Position = { -(width / 2), 0.0f, spikeConfig.ZPosition + 0.1f };
    playerConfig.Velocity = { 10.0f, 0.0f, 0.0f };
    playerConfig.Width = width / 20.0f;
    playerConfig.Height = playerConfig.Width * 1.3f;
    playerConfig.EnginePower = 0.1f;
    player = new Player(playerConfig);

    Reset();
}

Level::~Level()
{
    delete player;
    delete spikePool;
}

Player* Level::GetPlayer() const
{
    return player;
}

void Level::OnUpdate(st::Input* input, st::Timestep timestep)
{
    player->OnUpdate(input, timestep, GRAVITY);
    spikePool->OnUpdate(player->GetPosition());
    SetBackgroundPositions();
}

void Level::OnRender(st::Renderer* renderer)
{
    renderer->SubmitQuad(backgroundQuad);
    renderer->SubmitQuad(ceilingQuad);
    renderer->SubmitQuad(floorQuad);
    spikePool->OnRender(renderer);
    player->OnRender(renderer);
}

void Level::Reset()
{
    player->Reset();
    spikePool->Reset();
    SetBackgroundPositions();
}

void Level::SetBackgroundPositions()
{
    backgroundQuad.Position = { player->GetPosition().x, backgroundQuad.Position.y, backgroundQuad.Position.z };
    ceilingQuad.Position = { backgroundQuad.Position.x, ceilingQuad.Position.y, ceilingQuad.Position.z };
    floorQuad.Position = { backgroundQuad.Position.x, floorQuad.Position.y, floorQuad.Position.z };
}
