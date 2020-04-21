#include "Flappyship.h"


Flappyship::Flappyship(
        st::Window* window,
        st::Renderer* renderer,
        st::ImGuiRenderer* imGuiRenderer,
        st::OrthographicCameraController* cameraController,
        st::ResourceLoader* resourceLoader
)
        : Application(window, renderer, imGuiRenderer, cameraController), gameLayer(new GameLayer(renderer, resourceLoader))
{
    PushLayer(gameLayer);
}

Flappyship::~Flappyship()
{
    delete gameLayer;
}
