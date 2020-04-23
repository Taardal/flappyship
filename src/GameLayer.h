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
    st::Ref<st::Texture> triangleTexture;
    st::Quad backgroundQuad;
    st::Quad ceilingQuad;
    st::Quad floorQuad;
    Player* player;
    float gravity;

    float cameraWidth;
    float cameraHeight;
    float pillarWidth;
    float pillarHeight;
    float pillarWidthFactor;

    std::vector<st::Quad> pillars;
    int pillarForRecyclingIndex;
    float pillarRecyclingX;
    float recycledPillarX;

    st::Random random;
    uint32_t gapMin;
    uint32_t gapMax;
    uint32_t centerMin;
    uint32_t centerMax;
    float pillarGap;
    float pillarCenter;
    bool pillarCenterUp;

    GameState gameState;
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
    [[nodiscard]] float GetPillarY(const st::Quad& pillar) const;

    void RecyclePillars();
};


