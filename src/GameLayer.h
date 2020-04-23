#pragma once

#include "Player.h"
#include <storytime/Storytime.h>

class GameLayer : public st::Layer
{
private:
    st::Renderer* renderer;
    st::Input* input;
    st::OrthographicCameraController* cameraController;
    st::Ref<st::Texture> triangleTexture;
    st::Quad backgroundQuad;
    st::Quad ceilingQuad;
    st::Quad floorQuad;
    Player* player;
    float gravity;

    std::vector<st::Quad> pillars;
    int pillarForRecyclingIndex;
    float pillarRecyclingX;
    float pillarForRecyclingNextX;

public:
    explicit GameLayer(st::Renderer* renderer, st::Input* input, st::ResourceLoader* resourceLoader, st::OrthographicCameraController* cameraController);

    ~GameLayer() override;

    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(storytime::Timestep timestep) override;

    void OnImGuiUpdate() override;

    void OnEvent(const storytime::Event& event) override;
};


