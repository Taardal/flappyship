#include "GameLayer.h"

GameLayer::GameLayer(st::Renderer* renderer, st::Input* input, st::ResourceLoader* resourceLoader, st::OrthographicCameraController* cameraController)
        : st::Layer("GameLayer"), renderer(renderer), input(input), cameraController(cameraController)
{
    cameraController->SetZoomLevel(20.0f);
    cameraHeight = cameraController->GetZoomLevel() * 2;
    cameraWidth = cameraHeight / 12 * 16;

    backgroundQuad.Color = { 0.3f, 0.3f, 0.3f, 1.0f };
    backgroundQuad.Size = { cameraWidth, cameraHeight };
    backgroundQuad.Position = { 0.0f, 0.0f, 0.0f };

    ceilingQuad.Color = { 0.2f, 0.3f, 0.8f, 1.0f };
    ceilingQuad.Size = { backgroundQuad.Size.x, 5.0f };
    ceilingQuad.Position = { backgroundQuad.Position.x, backgroundQuad.Position.y + (backgroundQuad.Size.y / 2), backgroundQuad.Position.z + 0.1f };

    floorQuad.Color = ceilingQuad.Color;
    floorQuad.Size = { backgroundQuad.Size.x, ceilingQuad.Size.y };
    floorQuad.Position = { backgroundQuad.Position.x, backgroundQuad.Position.y - (backgroundQuad.Size.y / 2), ceilingQuad.Position.z };

    triangleTexture = resourceLoader->LoadTexture("triangle.png");

    st::Random random;
    uint32_t gapMin = 0;
    uint32_t gapMax = 3;
    float gap = random.UInt(gapMin, gapMax);

    uint32_t centerMin = 0;
    uint32_t centerMax = 10;
    float center = random.UInt(centerMin, centerMax);
    bool moveCenterUp = random.UInt(0, 1) == 1;

    pillarWidth = 30.0f;
    pillarHeight = cameraHeight;
    pillarWidthFactor = 0.8f;

    float pillarX = 0.0f;
    float pillarPairs = 4;
    pillars.resize(pillarPairs * 2);
    for (uint32_t i = 0; i < pillars.size(); i++)
    {
        st::Quad& pillar = pillars[i];
        pillar.Texture = triangleTexture;
        pillar.Color = { 0.2f, 0.8f, 0.2f, 1.0f };
        pillar.Size = { pillarWidth, pillarHeight };

        float topBottomGap = gap / 2;

        if (i % 2 == 0)
        {
            float pillarY = pillar.Position.y + (pillar.Size.y / 2) + topBottomGap;
            if (moveCenterUp)
            {
                pillarY += center;
            }
            else
            {
                pillarY -= center;
            }

            pillar.Position = { pillarX, pillarY, 0.0f };
            pillar.RotationInDegrees = 180.0f;
        }
        else
        {
            float pillarY = pillar.Position.y - (pillar.Size.y / 2) - topBottomGap;
            if (moveCenterUp)
            {
                pillarY += center;
            }
            else
            {
                pillarY -= center;
            }

            pillar.Position = { pillarX, pillarY, 0.0f };
            pillar.RotationInDegrees = 0.0f;
            pillarX += pillar.Size.x * pillarWidthFactor;

            gap = random.UInt(gapMin, gapMax);
            center = random.UInt(centerMin, centerMax);
            moveCenterUp = random.UInt(0, 1) == 1;
        }
    }
    st::Quad middlePillar = pillars[(pillars.size() / 2) - 1];
    st::Quad lastPillar = pillars[pillars.size() - 1];
    pillarRecyclingX = middlePillar.Position.x + (middlePillar.Size.x * pillarWidthFactor);
    pillarForRecyclingNextX = lastPillar.Position.x + (lastPillar.Size.x * pillarWidthFactor);
    pillarForRecyclingIndex = 0;

    float playerWidth = cameraWidth / 20.0f;
    float playerHeight = playerWidth * 1.3f;
    player = new Player();
    player->SetTexture(resourceLoader->LoadTexture("ship.png"));
    player->SetSize({ playerWidth, playerHeight });
    player->SetPosition({ -(cameraWidth / 2), 0.0f, 1.0f });
    player->SetVelocity({ 10.0f, 0.0f, 0.0f });
    player->SetEnginePower(0.1f);

    gravity = 0.05f;
}

GameLayer::~GameLayer()
{
    delete player;
}

void GameLayer::OnAttach()
{
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(st::Timestep timestep)
{
    st::OrthographicCamera* camera = cameraController->GetCamera();
    camera->SetPosition({ -player->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z });

    backgroundQuad.Position = { player->GetPosition().x, backgroundQuad.Position.y, 0.0f };
    renderer->SubmitQuad(backgroundQuad);

    ceilingQuad.Position = { backgroundQuad.Position.x, ceilingQuad.Position.y, backgroundQuad.Position.z + 0.1f };
    renderer->SubmitQuad(ceilingQuad);

    floorQuad.Position = { backgroundQuad.Position.x, floorQuad.Position.y, ceilingQuad.Position.z };
    renderer->SubmitQuad(floorQuad);

    if (player->GetPosition().x > pillarRecyclingX)
    {
        for (uint32_t i = 0; i < 2; i++)
        {
            st::Quad& pillar = pillars[pillarForRecyclingIndex];
            pillar.Position.x = pillarForRecyclingNextX;
            pillarForRecyclingIndex++;
        }
        if (pillarForRecyclingIndex > pillars.size() - 1)
        {
            pillarForRecyclingIndex = 0;
        }
        pillarRecyclingX += pillars[0].Size.x * pillarWidthFactor;
        pillarForRecyclingNextX += pillars[0].Size.x * pillarWidthFactor;
    }
    for (uint32_t i = 0; i < pillars.size(); i++)
    {
        st::Quad& pillar = pillars[i];
        if (pillar.RotationInDegrees == 180.0f)
        {
            pillar.Position = { pillar.Position.x, pillar.Position.y, ceilingQuad.Position.z + 0.1f };
        }
        else
        {
            pillar.Position = { pillar.Position.x, pillar.Position.y, floorQuad.Position.z + 0.1f };
        }
        renderer->SubmitQuad(pillar);
    }

    player->OnUpdate(renderer, input, timestep, gravity);
}

void GameLayer::OnImGuiUpdate()
{
    auto stats = renderer->GetStatistics();
    ImGui::Begin("Renderer statistics");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetVertexCount());
    ImGui::Text("Indices: %d", stats.GetIndexCount());
    ImGui::End();
}

void GameLayer::OnEvent(const storytime::Event& event)
{
    cameraController->OnEvent(event);
}
