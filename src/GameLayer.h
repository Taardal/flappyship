#pragma once

#include <storytime/Storytime.h>

class GameLayer : public st::Layer
{
private:
    st::Renderer* renderer;
    st::Ref<st::Texture> shipTexture;
    st::Ref<st::Texture> triangleTexture;

public:
    explicit GameLayer(st::Renderer* renderer, st::ResourceLoader* resourceLoader);

    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(storytime::Timestep timestep) override;

    void OnImGuiUpdate() override;

    void OnEvent(const storytime::Event& event) override;
};


