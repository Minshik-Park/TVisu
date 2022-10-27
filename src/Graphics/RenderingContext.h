#pragma once
#include "Common/TVisu.h"

namespace TVisu::Graphics
{
    class RenderingContext
    {
    public:
        RenderingContext() = default;
        virtual ~RenderingContext();
    };
}