#include "Flappyship.h"


Flappyship::Flappyship(
        st::Window* window,
        st::Renderer* renderer,
        st::ImGuiRenderer* imGuiRenderer,
        st::OrthographicCameraController* cameraController,
        st::ResourceLoader* resourceLoader,
        st::Input* input
)
        : Application(window, renderer, imGuiRenderer, cameraController)
{
    gameLayer = new GameLayer(renderer, input, resourceLoader, cameraController);
    PushLayer(gameLayer);
}

Flappyship::~Flappyship()
{
    delete gameLayer;
}
