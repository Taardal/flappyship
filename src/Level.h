#pragma once

#include "Player.h"
#include "SpikePool.h"
#include <storytime/Storytime.h>

class Level
{
private:
    static const float GRAVITY;

    st::Quad backgroundQuad;
    st::Quad ceilingQuad;
    st::Quad floorQuad;
    SpikePool* spikePool;
    Player* player;

public:
    explicit Level(st::ResourceLoader* resourceLoader, float width, float height);

    ~Level();

    [[nodiscard]] Player* GetPlayer() const;

    [[nodiscard]] uint32_t GetScore() const;

    void OnUpdate(st::Input* input, st::Timestep timestep);

    void OnRender(st::Renderer* renderer);

    void Reset();

private:
    void SetBackgroundPositions();
};


