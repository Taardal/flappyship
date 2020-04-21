#include "GameLayer.h"

GameLayer::GameLayer(st::Renderer* renderer)
        : st::Layer("GameLayer"), renderer(renderer)
{}

void GameLayer::OnAttach()
{

}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate(storytime::Timestep timestep)
{
    st::Quad quad;
    quad.Size = { 1.0f, 1.0f };
    quad.Position = { 0.0f, 0.0f, 0.0f };
    quad.Color = { 0.8f, 0.2f, 0.3f, 1.0f };
    renderer->DrawQuad(quad);
}

void GameLayer::OnImGuiUpdate()
{

}

void GameLayer::OnEvent(const storytime::Event& event)
{

}
