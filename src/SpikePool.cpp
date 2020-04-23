#include "SpikePool.h"

constexpr uint32_t SpikePool::SPIKE_PAIR_COUNT = 4;
constexpr uint32_t SpikePool::GAP_SIZE_MIN = 0;
constexpr uint32_t SpikePool::GAP_SIZE_MAX = 3;
constexpr uint32_t SpikePool::CENTER_SKEW_MIN = 0;
constexpr uint32_t SpikePool::CENTER_SKEW_MAX = 10;

SpikePool::SpikePool(const Config& config)
    : spikeWidth(config.Width), spikeWidthFactor(config.WidthFactor), spikeHeight(config.Height)
{
    spikes.resize(SPIKE_PAIR_COUNT * 2);
    for (uint32_t i = 0; i < spikes.size(); i++)
    {
        st::Quad& spike = spikes[i];
        spike.Texture = config.Texture;
        spike.Color = { 0.2f, 0.8f, 0.2f, 1.0f };
        spike.Size = { spikeWidth, spikeHeight };
        spike.Position = { 0.0f, 0.0f, config.ZPosition };
        spike.RotationInDegrees = i % 2 == 0 ? 180.0f : 0.0f;
    }
    SetNextGapSize();
    SetNextCenterPoint();
    Reset();
}

void SpikePool::OnUpdate(const glm::vec3& playerPosition)
{
    if (playerPosition.x > nextTriggerX)
    {
        for (uint32_t i = 0; i < 2; i++)
        {
            st::Quad& spike = spikes[nextIndex];
            spike.Position.x = nextX;
            spike.Position.y = GetY(spike);
            nextIndex++;
        }
        if (nextIndex > spikes.size() - 1)
        {
            nextIndex = 0;
        }
        SetNextPositions();
        SetNextGapSize();
        SetNextCenterPoint();
    }
}

void SpikePool::OnRender(st::Renderer* renderer)
{
    for (auto& pillar : spikes)
    {
        renderer->SubmitQuad(pillar);
    }
}

void SpikePool::Reset()
{
    float x = 0.0f;
    for (uint32_t i = 0; i < spikes.size(); i++)
    {
        st::Quad& spike = spikes[i];
        if (i % 2 == 0)
        {
            spike.Position = { x, GetY(spike), spike.Position.z };
        }
        else
        {
            spike.Position = { x, GetY(spike), spike.Position.z };
            x += spike.Size.x * spikeWidthFactor;
            SetNextGapSize();
            SetNextCenterPoint();
        }
    }
    st::Quad middleSpike = spikes[(spikes.size() / 2) - 1];
    nextTriggerX = middleSpike.Position.x + (middleSpike.Size.x * spikeWidthFactor);
    st::Quad lastSpike = spikes[spikes.size() - 1];
    nextX = lastSpike.Position.x + (lastSpike.Size.x * spikeWidthFactor);
    nextIndex = 0;
}

void SpikePool::SetNextGapSize()
{
    gapSize = random.UInt(GAP_SIZE_MIN, GAP_SIZE_MAX);
}

void SpikePool::SetNextCenterPoint()
{
    centerSkew = random.UInt(CENTER_SKEW_MIN, CENTER_SKEW_MAX);
    skewCenterUpwards = random.UInt(0, 1) == 1;
}

void SpikePool::SetNextPositions()
{
    nextX += spikes[0].Size.x * spikeWidthFactor;
    nextTriggerX += spikes[0].Size.x * spikeWidthFactor;
}

float SpikePool::GetY(const st::Quad& spike) const
{
    float y = spike.Size.y / 2;
    if (spike.RotationInDegrees == 180.0f)
    {
        y = y + (gapSize / 2);
    }
    else
    {
        y = -y - (gapSize / 2);
    }
    if (skewCenterUpwards)
    {
        y += centerSkew;
    }
    else
    {
        y -= centerSkew;
    }
    return y;
}