#include "GameLayer.h"

GameLayer::GameLayer(st::Renderer* renderer, st::Input* input, st::ResourceLoader* resourceLoader, st::OrthographicCameraController* cameraController)
        : st::Layer("GameLayer"), renderer(renderer), input(input), cameraController(cameraController)
{
    cameraController->SetZoomLevel(1.5f);

    player = new Player();
    player->SetTexture(resourceLoader->LoadTexture("ship.png"));
    player->SetSize({ 0.1f, 0.13f });
    player->SetPosition({ 0.0f, 0.0f, 1.0f });
    player->SetVelocity({ 0.5f, 2.0f, 0.0f });
    player->SetEnginePower(0.025f);

    triangleTexture = resourceLoader->LoadTexture("triangle.png");

    gravity = 0.01f;

    pillars.resize(8);
    for (uint32_t i = 0; i < pillars.size(); i++)
    {
        Pillar& pillar = pillars[i];

        pillar.TopScale = { 1.0f, 2.5f };
        pillar.TopPosition.x = i;
        pillar.TopPosition.y = 1.25f;
        pillar.TopPosition.z = 0.5f;

        pillar.BottomScale = { 1.0f, 2.5f };
        pillar.BottomPosition.x = i;
        pillar.BottomPosition.y = -1.25f;
        pillar.BottomPosition.z = 0.5f;

        float center = st::Random::Float() * 2.5f - 1.5f;
        float gap = 1.25f + st::Random::Float() * 2.5f;

        pillar.TopPosition.y = 1.25f - ((1.25f - center) * 0.5f) + gap * 0.1f;
        pillar.BottomPosition.y = -1.25f - ((-1.25f - center) * 0.5f) - gap * 0.1f;
    }

    pillarIndex = 0;
    pillarTarget = pillars[(pillars.size() / 2) - 1].TopPosition.x + pillars[(pillars.size() / 2) - 1].TopScale.x;
    nextPillarX = pillars[pillars.size() - 1].TopPosition.x + pillars[pillars.size() - 1].TopScale.x;
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
    if (player->GetPosition().x > pillarTarget)
	{
        Pillar& pillar = pillars[pillarIndex];

        pillar.TopScale = { 1.0f, 2.5f };
        pillar.TopPosition.x = nextPillarX;
        pillar.TopPosition.y = 1.25f;
        pillar.TopPosition.z = 0.5f;

        pillar.BottomScale = { 1.0f, 2.5f };
        pillar.BottomPosition.x = nextPillarX;
        pillar.BottomPosition.y = -1.25f;
        pillar.BottomPosition.z = 0.5f;

        float center = st::Random::Float() * 2.5f - 1.5f;
        float gap = 1.25f + st::Random::Float() * 2.5f;

        pillar.TopPosition.y = 1.25f - ((1.25f - center) * 0.5f) + gap * 0.1f;
        pillar.BottomPosition.y = -1.25f - ((-1.25f - center) * 0.5f) - gap * 0.1f;

		//pillarIndex = pillarIndex++ % (int32_t) pillars.size();
		pillarTarget += 1.0f;
        nextPillarX += 1.0f;
		pillarIndex++;
		if (pillarIndex > pillars.size() - 1)
        {
		    pillarIndex = 0;
        }
	}

    st::OrthographicCamera* camera = cameraController->GetCamera();
    auto cameraX = camera->GetPosition().x - player->GetVelocity().x * (float) timestep;
    camera->SetPosition({ cameraX, camera->GetPosition().y, camera->GetPosition().z });

    st::Quad backgroundQuad{};
    backgroundQuad.Position = { 0.0f, 0.0f, 0.0f };
    backgroundQuad.Size = { 25.0f, 2.5f };
    backgroundQuad.Color = { 0.3f, 0.3f, 0.3f, 1.0f };
    renderer->SubmitQuad(backgroundQuad);

    st::Quad ceilingQuad{};
    ceilingQuad.Size = { 25.0f, 1.0f };
    ceilingQuad.Position = { backgroundQuad.Position.x, backgroundQuad.Position.y + (backgroundQuad.Size.y / 2), 0.0f };
    ceilingQuad.Color = { 0.2f, 0.3f, 0.8f, 1.0f };
    renderer->SubmitQuad(ceilingQuad);

    st::Quad floorQuad{};
    floorQuad.Position = { backgroundQuad.Position.x, backgroundQuad.Position.y - (backgroundQuad.Size.y / 2), 0.0f };
    floorQuad.Size = { 25.0f, 1.0f };
    floorQuad.Color = { 0.2f, 0.3f, 0.8f, 1.0f };
    renderer->SubmitQuad(floorQuad);

    for (auto& pillar : pillars)
    {
        st::Quad quad{};
        quad.Texture = triangleTexture;
        quad.Color = { 0.2f, 0.8f, 0.2f, 1.0f };

        quad.Position = pillar.TopPosition;
        quad.Size = pillar.TopScale;
        quad.RotationInDegrees = 180.0f;
        //renderer->SubmitQuad(quad);

        quad.Position = pillar.BottomPosition;
        quad.Size = pillar.BottomScale;
        quad.RotationInDegrees = 0.0f;
        //renderer->SubmitQuad(quad);
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
