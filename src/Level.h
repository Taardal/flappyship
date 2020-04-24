#pragma once

#include "Player.h"
#include "SpikeManager.h"
#include <storytime/Storytime.h>

class Level
{
private:
    static const float GRAVITY;

    float width;
    float height;
    glm::vec3 environmentColorHSV;
    st::Quad backgroundQuad;
    st::Quad ceilingQuad;
    st::Quad floorQuad;
    SpikeManager* spikeManager;
    Player* player;

public:
    explicit Level(st::ResourceLoader* resourceLoader, float width, float height);

    ~Level();

    [[nodiscard]] Player* GetPlayer() const;

    [[nodiscard]] uint32_t GetScore() const;

    void OnUpdate(st::Input* input, st::Timestep timestep);

    void OnRender(st::Renderer* renderer);

    void Reset();

    [[nodiscard]] bool IsCollision() const;

private:
    void SetBackgroundPositions();

    void SetEnvironmentColors(const st::Timestep& timestep);

    static bool IsPlayerVertexWithinSpikeVertices(const glm::vec4& playerVertex, glm::vec4* spikeVertices);

    static glm::vec4 GetRGBA(glm::vec3 hsv);
};


