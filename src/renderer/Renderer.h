#pragma once

#include "core/utils/logger.h"

class Renderer
{
public:
    Renderer() = default;
    virtual ~Renderer() = default;

    virtual bool BeginFrame(float deltaTime) = 0;
    virtual bool EndFrame(float deltaTime) = 0;
    virtual void Resize(int width, int height) = 0;
};