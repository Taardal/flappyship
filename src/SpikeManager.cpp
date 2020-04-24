#include "SpikeManager.h"

constexpr uint32_t SpikeManager::SPIKE_PAIR_COUNT = 4;
constexpr uint32_t SpikeManager::GAP_SIZE_MIN = 0;
constexpr uint32_t SpikeManager::GAP_SIZE_MAX = 3;
constexpr uint32_t SpikeManager::CENTER_SKEW_MIN = 0;
constexpr uint32_t SpikeManager::CENTER_SKEW_MAX = 10;

SpikeManager::SpikeManager(const Config& config)
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

uint32_t SpikeManager::GetSpikeWidth() const
{
    return spikeWidth * spikeWidthFactor;
}

void SpikeManager::OnUpdate(const glm::vec3& playerPosition)
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

void SpikeManager::OnRender(st::Renderer* renderer)
{
    for (auto& pillar : spikes)
    {
        renderer->SubmitQuad(pillar);
    }
}

void SpikeManager::Reset()
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
    nextTriggerX = spikes[(spikes.size() / 2) - 1].Position.x + GetSpikeWidth();
    nextX = spikes[spikes.size() - 1].Position.x + GetSpikeWidth();
    nextIndex = 0;
}

void SpikeManager::SetNextGapSize()
{
    gapSize = random.UInt(GAP_SIZE_MIN, GAP_SIZE_MAX);
}

void SpikeManager::SetNextCenterPoint()
{
    centerSkew = random.UInt(CENTER_SKEW_MIN, CENTER_SKEW_MAX);
    skewCenterUpwards = random.UInt(0, 1) == 1;
}

void SpikeManager::SetNextPositions()
{
    nextX += GetSpikeWidth();
    nextTriggerX += GetSpikeWidth();
}

float SpikeManager::GetY(const st::Quad& spike) const
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

const std::vector<st::Quad>& SpikeManager::GetSpikes() const
{
    return spikes;
}

void SpikeManager::FillTransformedVertices(glm::vec4* vertices, const st::Quad& spike)
{
    vertices[0] = { -0.5f + 0.1f, -0.5f + 0.1f, 0.0f, 1.0f };
    vertices[1] = {  0.5f - 0.1f, -0.5f + 0.1f, 0.0f, 1.0f };
    vertices[2] = {  0.0f + 0.0f,  0.5f - 0.1f, 0.0f, 1.0f };
    for (int i = 0; i < 3; i++)
    {
        const glm::mat4& translation = glm::translate(glm::mat4(1.0f), { spike.Position.x, spike.Position.y, 0.0f });
        const glm::mat4& rotation = glm::rotate(glm::mat4(1.0f), glm::radians(spike.RotationInDegrees), { 0.0f, 0.0f, 1.0f });
        const glm::mat4& scale = glm::scale(glm::mat4(1.0f), { spike.Size.x, spike.Size.y, 1.0f });
        vertices[i] = translation * rotation * scale * vertices[i];
    }
}