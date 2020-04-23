#include "GameLayer.h"

GameLayer::GameLayer(st::Renderer* renderer, st::Input* input, st::ResourceLoader* resourceLoader, st::OrthographicCameraController* cameraController)
        : st::Layer("GameLayer"), renderer(renderer), input(input), cameraController(cameraController)
{
    cameraController->SetZoomLevel(20.0f);
    cameraHeight = cameraController->GetZoomLevel() * 2;
    cameraWidth = cameraHeight / 12 * 16;

    level = new Level(resourceLoader);

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

    gapMin = 0;
    gapMax = 3;
    pillarGap = random.UInt(gapMin, gapMax);

    centerMin = 0;
    centerMax = 10;
    pillarCenter = random.UInt(centerMin, centerMax);
    pillarCenterUp = random.UInt(0, 1) == 1;

    pillarWidth = 30.0f;
    pillarHeight = cameraHeight;
    pillarWidthFactor = 0.9f;

    float pillarX = 0.0f;
    float pillarPairs = 4;
    pillars.resize(pillarPairs * 2);
    for (uint32_t i = 0; i < pillars.size(); i++)
    {
        st::Quad& pillar = pillars[i];
        pillar.Texture = triangleTexture;
        pillar.Color = { 0.2f, 0.8f, 0.2f, 1.0f };
        pillar.Size = { pillarWidth, pillarHeight };
        if (i % 2 == 0)
        {
            pillar.RotationInDegrees = 180.0f;
            pillar.Position = { pillarX, GetPillarY(pillar), ceilingQuad.Position.z + 0.1f };
        }
        else
        {
            pillar.RotationInDegrees = 0.0f;
            pillar.Position = { pillarX, GetPillarY(pillar), floorQuad.Position.z + 0.1f };
            pillarX += pillar.Size.x * pillarWidthFactor;
            pillarGap = random.UInt(gapMin, gapMax);
            pillarCenter = random.UInt(centerMin, centerMax);
            pillarCenterUp = random.UInt(0, 1) == 1;
        }
    }
    st::Quad middlePillar = pillars[(pillars.size() / 2) - 1];
    st::Quad lastPillar = pillars[pillars.size() - 1];
    pillarRecyclingX = middlePillar.Position.x + (middlePillar.Size.x * pillarWidthFactor);
    recycledPillarX = lastPillar.Position.x + (lastPillar.Size.x * pillarWidthFactor);
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

    gameState = GameState::Start;

    st::OrthographicCamera* camera = cameraController->GetCamera();
    camera->SetPosition({ -player->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z });

    backgroundQuad.Position = { player->GetPosition().x, backgroundQuad.Position.y, backgroundQuad.Position.z };
    ceilingQuad.Position = { backgroundQuad.Position.x, ceilingQuad.Position.y, ceilingQuad.Position.z };
    floorQuad.Position = { backgroundQuad.Position.x, floorQuad.Position.y, floorQuad.Position.z };
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
    switch (gameState)
    {
        case GameState::Start:
        {
            if (input->IsKeyPressed(st::KeyCode::KEY_ENTER))
            {
                gameState = GameState::Play;
            }
            break;
        }
        case GameState::Play:
        {
            if (player->GetPosition().y < -10.0f || player->GetPosition().y > 10.0f)
            {
                gameState = GameState::End;
            }

            st::OrthographicCamera* camera = cameraController->GetCamera();
            camera->SetPosition({ -player->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z });

            backgroundQuad.Position = { player->GetPosition().x, backgroundQuad.Position.y, backgroundQuad.Position.z };
            ceilingQuad.Position = { backgroundQuad.Position.x, ceilingQuad.Position.y, ceilingQuad.Position.z };
            floorQuad.Position = { backgroundQuad.Position.x, floorQuad.Position.y, floorQuad.Position.z };

            if (player->GetPosition().x > pillarRecyclingX)
            {
                RecyclePillars();
            }

            player->OnUpdate(renderer, input, timestep, gravity);
            break;
        }
        default:
        {
            if (input->IsKeyPressed(st::KeyCode::KEY_R))
            {
                player->SetPosition({ -(cameraWidth / 2), 0.0f, 1.0f });
                player->SetVelocity({ 10.0f, 0.0f, 0.0f });
                player->SetRotationInDegrees(0);

                st::OrthographicCamera* camera = cameraController->GetCamera();
                camera->SetPosition({ -player->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z });

                backgroundQuad.Position = { player->GetPosition().x, backgroundQuad.Position.y, backgroundQuad.Position.z };
                ceilingQuad.Position = { backgroundQuad.Position.x, ceilingQuad.Position.y, ceilingQuad.Position.z };
                floorQuad.Position = { backgroundQuad.Position.x, floorQuad.Position.y, floorQuad.Position.z };

                gameState = GameState::Start;
            }
        }
    }
    renderer->SubmitQuad(backgroundQuad);
    renderer->SubmitQuad(ceilingQuad);
    renderer->SubmitQuad(floorQuad);
    for (auto& pillar : pillars)
    {
        renderer->SubmitQuad(pillar);
    }
    player->OnRender(renderer);
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

    ImGui::Begin("Flappyship");
    switch (gameState)
    {
        case GameState::Start:
        {
            ImGui::Text("Click to play!");
            break;
        }
        case GameState::Play:
        {
            ImGui::Text("Score: 10");
            break;
        }
        default:
        {
            ImGui::Text("Game over!");
        }
    }
    ImGui::End();
}

void GameLayer::OnEvent(const storytime::Event& event)
{
    cameraController->OnEvent(event);
}

void GameLayer::RecyclePillars()
{
    for (uint32_t i = 0; i < 2; i++)
    {
        st::Quad& pillar = pillars[pillarForRecyclingIndex];
        pillar.Position.x = recycledPillarX;
        pillar.Position.y = GetPillarY(pillar);
        pillarForRecyclingIndex++;
    }
    if (pillarForRecyclingIndex > pillars.size() - 1)
    {
        pillarForRecyclingIndex = 0;
    }
    pillarRecyclingX += pillars[0].Size.x * pillarWidthFactor;
    recycledPillarX += pillars[0].Size.x * pillarWidthFactor;
    pillarGap = random.UInt(gapMin, gapMax);
    pillarCenter = random.UInt(centerMin, centerMax);
    pillarCenterUp = random.UInt(0, 1) == 1;
}

float GameLayer::GetPillarY(const st::Quad& pillar) const
{
    float pillarY;
    if (pillar.RotationInDegrees == 180.0f)
    {
        pillarY = (pillar.Size.y / 2) + (pillarGap / 2);
    }
    else
    {
        pillarY = -(pillar.Size.y / 2) - (pillarGap / 2);
    }
    pillarY += pillarCenterUp ? pillarCenter : -pillarCenter;
    return pillarY;
}
