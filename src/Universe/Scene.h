#pragma once
#include "Common/TVisu.h"

namespace TVisu::Universe
{
    class Scene
    {
    public:
        Scene() = default;
        virtual ~Scene();

        result_t Update();
        result_t Render();

    private:
    };
}
