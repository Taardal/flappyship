#include "GameLayer.h"

GameLayer::GameLayer(st::Renderer* renderer, st::Input* input, st::ResourceLoader* resourceLoader, st::OrthographicCameraController* cameraController)
        : st::Layer("GameLayer"), renderer(renderer), input(input), cameraController(cameraController), gameState(GameState::Start)
{
    cameraController->SetZoomLevel(20.0f);
    cameraHeight = cameraController->GetZoomLevel() * 2;
    cameraWidth = cameraHeight / 12 * 16;
    level = new Level(resourceLoader, cameraWidth, cameraHeight);
}

GameLayer::~GameLayer()
{
    delete level;
}

void GameLayer::OnAttach()
{
    SetCameraPosition();
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
            if (level->IsCollision())
            {
                gameState = GameState::End;
            }
            else
            {
                level->OnUpdate(input, timestep);
            }
            break;
        }
        default:
        {
            if (input->IsKeyPressed(st::KeyCode::KEY_R))
            {
                level->Reset();
                gameState = GameState::Start;
            }
        }
    }
    SetCameraPosition();
    level->OnRender(renderer);
}

void GameLayer::OnImGuiUpdate()
{
    auto stats = renderer->GetStatistics();
    ImGui::Begin("Rendering");
    ImGui::Text("Draw calls: %d", stats.DrawCalls);
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
            ImGui::Text("Score: %d", level->GetScore());
            break;
        }
        default:
        {
            ImGui::Text("Game over!");
            ImGui::Text("Score: %d", level->GetScore());
        }
    }
    ImGui::End();
}

void GameLayer::OnEvent(const storytime::Event& event)
{
    cameraController->OnEvent(event);
}

void GameLayer::SetCameraPosition() const
{
    st::OrthographicCamera* camera = cameraController->GetCamera();
    camera->SetPosition({ -level->GetPlayer()->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z });

}
