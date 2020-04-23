#pragma once

#include "Player.h"
#include <storytime/Storytime.h>

class Level
{
public:
    explicit Level(st::ResourceLoader* pLoader);

    void OnUpdate();

    void OnRender(st::Renderer* renderer);

    void Reset();
};


