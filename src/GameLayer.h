#pragma once

#include "Player.h"
#include <storytime/Storytime.h>

struct Pillar
{
    glm::vec3 TopPosition;
    glm::vec2 TopScale;
    glm::vec3 BottomPosition;
    glm::vec2 BottomScale;
};

class GameLayer : public st::Layer
{
private:
    st::Renderer* renderer;
    st::Input* input;
    st::OrthographicCameraController* cameraController;
    st::Ref<st::Texture> triangleTexture;
    Player* player;
    float gravity;

    std::vector<Pillar> pillars;
    int pillarIndex;
    float pillarTarget;
    float nextPillarX;

public:
    explicit GameLayer(st::Renderer* renderer, st::Input* input, st::ResourceLoader* resourceLoader, st::OrthographicCameraController* cameraController);

    ~GameLayer() override;

    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(storytime::Timestep timestep) override;

    void OnImGuiUpdate() override;

    void OnEvent(const storytime::Event& event) override;
};


