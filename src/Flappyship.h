#pragma once

#include "GameLayer.h"
#include <storytime/Storytime.h>


class Flappyship : public st::Application
{
private:
    GameLayer* gameLayer;

public:
    Flappyship(
            st::Window* window,
            st::Renderer* renderer,
            st::ImGuiRenderer* imGuiRenderer,
            st::OrthographicCameraController* cameraController,
            st::ResourceLoader* resourceLoader,
            st::Input* input
    );

    ~Flappyship();
};

