#pragma once

#include <storytime/Storytime.h>

class GameLayer : public st::Layer
{
private:
    st::Renderer* renderer;

public:
    explicit GameLayer(st::Renderer* renderer);

    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(storytime::Timestep timestep) override;

    void OnImGuiUpdate() override;

    void OnEvent(const storytime::Event& event) override;
};


