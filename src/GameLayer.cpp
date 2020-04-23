#include "GameLayer.h"

GameLayer::GameLayer(st::Renderer* renderer, st::Input* input, st::ResourceLoader* resourceLoader, st::OrthographicCameraController* cameraController)
        : st::Layer("GameLayer"), renderer(renderer), input(input), cameraController(cameraController)
{
    cameraController->SetZoomLevel(20.0f);
    float cameraHeight = cameraController->GetZoomLevel() * 2;
    float cameraWidth = cameraHeight / 12 * 16;

    backgroundQuad.Size = { cameraWidth, cameraHeight };
    backgroundQuad.Color = { 0.3f, 0.3f, 0.3f, 1.0f };

    ceilingQuad.Size = { backgroundQuad.Size.x, 5.0f };
    ceilingQuad.Color = { 0.2f, 0.3f, 0.8f, 1.0f };

    floorQuad.Size = { backgroundQuad.Size.x, ceilingQuad.Size.y };
    floorQuad.Color = ceilingQuad.Color;

    triangleTexture = resourceLoader->LoadTexture("triangle.png");

    float pillarX = 0.0f;
    float pillarPairs = 6;
    pillars.resize(pillarPairs * 2);
    for (uint32_t i = 0; i < pillars.size(); i++)
    {
        st::Quad& pillar = pillars[i];
        pillar.Texture = triangleTexture;
        pillar.Color = { 0.2f, 0.8f, 0.2f, 1.0f };
        pillar.Size = { 10.0f, (cameraHeight / 2) * 0.9f };
        pillar.Position = { pillarX, 0.0f, ceilingQuad.Position.z + 0.1f };
        if (i % 2 == 0)
        {
            pillar.RotationInDegrees = 180.0f;
        }
        else
        {
            pillar.RotationInDegrees = 0.0f;
            pillarX += pillar.Size.x;
        }
    }
    st::Quad middlePillar = pillars[(pillars.size() / 2) - 1];
    st::Quad lastPillar = pillars[pillars.size() - 1];
    pillarRecyclingX = middlePillar.Position.x + middlePillar.Size.x;
    pillarForRecyclingNextX = lastPillar.Position.x + lastPillar.Size.x;
    pillarForRecyclingIndex = 0;

    /*
        float center = st::Random::Float() * 2.5f - 1.5f;
        float gap = 1.25f + st::Random::Float() * 2.5f;

        pillar.TopPosition.y = 1.25f - ((1.25f - center) * 0.5f) + gap * 0.1f;
        pillar.BottomPosition.y = -1.25f - ((-1.25f - center) * 0.5f) - gap * 0.1f;
    */

    float playerWidth = cameraWidth / 20.0f;
    float playerHeight = playerWidth * 1.3f;
    player = new Player();
    player->SetTexture(resourceLoader->LoadTexture("ship.png"));
    player->SetSize({ playerWidth, playerHeight });
    player->SetPosition({ 0.0f, 0.0f, 1.0f });
    player->SetVelocity({ 5.0f, 0.0f, 0.0f });
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

    backgroundQuad.Position = { player->GetPosition().x, 0.0f, 0.0f };
    renderer->SubmitQuad(backgroundQuad);

    ceilingQuad.Position = { backgroundQuad.Position.x, backgroundQuad.Position.y + (backgroundQuad.Size.y / 2), backgroundQuad.Position.z + 0.1f };
    renderer->SubmitQuad(ceilingQuad);

    floorQuad.Position = { backgroundQuad.Position.x, backgroundQuad.Position.y - (backgroundQuad.Size.y / 2), ceilingQuad.Position.z };
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
        pillarRecyclingX += pillars[0].Size.x;
        pillarForRecyclingNextX += pillars[0].Size.x;
    }
    for (uint32_t i = 0; i < pillars.size(); i++)
    {
        st::Quad& pillar = pillars[i];
        if (pillar.RotationInDegrees == 180.0f)
        {
            pillar.Position = { pillar.Position.x, ceilingQuad.Position.y - (pillar.Size.y / 4), ceilingQuad.Position.z + 0.1f };
        }
        else
        {
            pillar.Position = { pillar.Position.x, floorQuad.Position.y + (pillar.Size.y / 4), ceilingQuad.Position.z + 0.1f };
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
