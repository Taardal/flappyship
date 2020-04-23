#include "Flappyship.h"
#include <storytime/Storytime.h>

int main()
{
    st::Window::Config windowConfig = {};
    windowConfig.Title = "Flappyship";
    windowConfig.Width = 1280;
    windowConfig.Height = windowConfig.Width / 16 * 12;
    windowConfig.Maximized = false;

    st::GraphicsContext::Config graphicsConfig = {};
    graphicsConfig.VersionMajor = 4;
    graphicsConfig.VersionMinor = 1;
    graphicsConfig.GlslVersion = "#version 410";

    st::Engine::Config engineConfig = {};
    engineConfig.logLevel = st::LogLevel::Trace;
    engineConfig.windowConfig = windowConfig;
    engineConfig.graphicsConfig = graphicsConfig;

    auto engine = new st::Engine(engineConfig);
    engine->Run();
    delete engine;
}

st::Application* CreateApplication(
        st::Window* window,
        st::Renderer* renderer,
        st::ImGuiRenderer* imGuiRenderer,
        st::Input* input,
        st::OrthographicCameraController* cameraController,
        st::ResourceLoader* resourceLoader
)
{
    return new Flappyship(window, renderer, imGuiRenderer, cameraController, resourceLoader, input);
}