#pragma once

#include "Level.h"
#include "Player.h"
#include <storytime/Storytime.h>

enum class GameState
{
    None = 0,
    Start = 1,
    Play = 2,
    End = 3
};

class GameLayer : public st::Layer
{
private:
    st::Renderer* renderer;
    st::Input* input;
    st::OrthographicCameraController* cameraController;
    GameState gameState;
    float cameraWidth;
    float cameraHeight;
    Level* level;

public:
    explicit GameLayer(st::Renderer* renderer, st::Input* input, st::ResourceLoader* resourceLoader, st::OrthographicCameraController* cameraController);

    ~GameLayer() override;

    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(storytime::Timestep timestep) override;

    void OnImGuiUpdate() override;

    void OnEvent(const storytime::Event& event) override;

private:
    void SetCameraPosition() const;
};


