#include "GameLayer.h"

GameLayer::GameLayer(st::Renderer* renderer, st::ResourceLoader* resourceLoader)
        : st::Layer("GameLayer"), renderer(renderer)
{
    shipTexture = resourceLoader->LoadTexture("puppy.png");
    triangleTexture = resourceLoader->LoadTexture("triangle.png");
}

void GameLayer::OnAttach()
{

}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(storytime::Timestep timestep)
{
    st::Quad quad{};
    quad.Size = { 0.1f, 0.13f };
    quad.Position = { 0.0f, 0.0f, 0.0f };
    quad.Texture = shipTexture;
    renderer->DrawQuad(quad);


    st::Quad quad1{};
    quad.Size = { 0.1f, 0.1f };
    quad.Position = { -0.5f, -0.5f, 0.0f };
    quad.Texture = triangleTexture;
    renderer->DrawQuad(quad);
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

}
