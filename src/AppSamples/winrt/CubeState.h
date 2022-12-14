#pragma once
#include "Universe/StateBase.h"

namespace TVisu
{
    /// <summary>
    /// Sample state of rendering single rotating cube
    /// </summary>
    class CubeState : public Universe::StateBase
    {
    public:
        CubeState(const uint32_t id);
        virtual ~CubeState();

        virtual result_t Enter() override;
        virtual result_t Exit() override;

        virtual result_t Update() override;
        virtual result_t Render() override;
    };
}