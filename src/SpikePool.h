#pragma once

#include <storytime/Storytime.h>

class SpikePool
{
public:
    struct Config
    {
        st::Ref<st::Texture> Texture;
        float Width;
        float WidthFactor;
        float Height;
        float ZPosition;
    };

private:
    static const uint32_t SPIKE_PAIR_COUNT;
    static const uint32_t GAP_SIZE_MIN;
    static const uint32_t GAP_SIZE_MAX;
    static const uint32_t CENTER_SKEW_MIN;
    static const uint32_t CENTER_SKEW_MAX;

    float spikeWidth;
    float spikeWidthFactor;
    float spikeHeight;
    std::vector<st::Quad> spikes;
    st::Random random;
    uint32_t nextIndex = 0;
    float nextTriggerX = 0.0f;
    float nextX = 0.0f;
    float gapSize = 0.0f;
    float centerSkew = 0.0f;
    bool skewCenterUpwards = false;

public:
    explicit SpikePool(const Config& config);

    [[nodiscard]] uint32_t GetSpikeWidth() const;

    void OnUpdate(const glm::vec3& playerPosition);

    void OnRender(st::Renderer* renderer);

    void Reset();

private:
    [[nodiscard]] float GetY(const st::Quad& spike) const;

    void SetNextGapSize();

    void SetNextCenterPoint();

    void SetNextPositions();
};


