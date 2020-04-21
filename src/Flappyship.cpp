#include "Flappyship.h"


Flappyship::Flappyship(st::Window* window, st::Renderer* renderer, st::ImGuiRenderer* imGuiRenderer, st::OrthographicCameraController* cameraController)
        : Application(window, renderer, imGuiRenderer, cameraController), gameLayer(new GameLayer(renderer))
{
    PushLayer(gameLayer);
}

Flappyship::~Flappyship()
{
    delete gameLayer;
}
