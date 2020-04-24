#include "Level.h"

constexpr float Level::GRAVITY = 0.05f;

Level::Level(st::ResourceLoader* resourceLoader, float width, float height)
        : width(width), height(height)
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

    SpikeManager::Config spikeConfig = {};
    spikeConfig.Texture = resourceLoader->LoadTexture("triangle.png");
    spikeConfig.Width = 30.0f;
    spikeConfig.WidthFactor = 0.9f;
    spikeConfig.Height = height;
    spikeConfig.ZPosition = ceilingQuad.Position.z + 0.1f;
    spikeManager = new SpikeManager(spikeConfig);

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
    delete spikeManager;
}

Player* Level::GetPlayer() const
{
    return player;
}

uint32_t Level::GetScore() const
{
    float spikeWidth = spikeManager->GetSpikeWidth();
    return (player->GetPosition().x + spikeWidth) / spikeWidth;
}

void Level::OnUpdate(st::Input* input, st::Timestep timestep)
{
    player->OnUpdate(input, timestep, GRAVITY);
    spikeManager->OnUpdate(player->GetPosition());
    SetBackgroundPositions();
}

void Level::OnRender(st::Renderer* renderer)
{
    renderer->SubmitQuad(backgroundQuad);
    renderer->SubmitQuad(ceilingQuad);
    renderer->SubmitQuad(floorQuad);
    spikeManager->OnRender(renderer);
    player->OnRender(renderer);
}

void Level::Reset()
{
    player->Reset();
    spikeManager->Reset();
    SetBackgroundPositions();
}

void Level::SetBackgroundPositions()
{
    backgroundQuad.Position = { player->GetPosition().x, backgroundQuad.Position.y, backgroundQuad.Position.z };
    ceilingQuad.Position = { backgroundQuad.Position.x, ceilingQuad.Position.y, ceilingQuad.Position.z };
    floorQuad.Position = { backgroundQuad.Position.x, floorQuad.Position.y, floorQuad.Position.z };
}

bool Level::IsCollision() const
{
    bool ceilingCollision = player->GetPosition().y + (player->GetSize().y / 2) > ceilingQuad.Position.y - (ceilingQuad.Size.y / 2);
    bool floorCollision = player->GetPosition().y - (player->GetSize().y / 2) < floorQuad.Position.y + (floorQuad.Size.y / 2);
    if (ceilingCollision || floorCollision)
    {
        return true;
    }
    glm::vec4 playerVertices[4];
    player->FillTransformedVertices(playerVertices);
    for (const st::Quad& spike : spikeManager->GetSpikes())
    {
        glm::vec4 spikeVertices[3];
        spikeManager->FillTransformedVertices(spikeVertices, spike);
        for (glm::vec4& playerVertex : playerVertices)
        {
            if (IsPlayerVertexWithinSpikeVertices(playerVertex, spikeVertices))
            {
                return true;
            }
        }
    }
    return false;
}

bool Level::IsPlayerVertexWithinSpikeVertices(const glm::vec4& playerVertex, glm::vec4* spikeVertices)
{
    const glm::vec4& p = playerVertex;
    const glm::vec4& p0 = spikeVertices[0];
    const glm::vec4& p1 = spikeVertices[1];
    const glm::vec4& p2 = spikeVertices[2];

    float s = p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y;
    float t = p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y;

    if ((s < 0) != (t < 0))
        return false;

    float A = -p1.y * p2.x + p0.y * (p2.x - p1.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;

    return A < 0 ?
           (s <= 0 && s + t >= A) :
           (s >= 0 && s + t <= A);
}