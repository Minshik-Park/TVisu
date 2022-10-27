#pragma once
#include "Common/TVisu.h"
#include "StateBase.h"

namespace TVisu::Universe
{
    /// <summary>
    /// Describe world class.
    /// 
    /// World class maintain state of world and manage scenes to render.
    /// World manage objects in the world and provide renderable objects to the current scene to render.
    /// 
    /// </summary>
    class World
    {
    public:
        World() = default;
        virtual ~World();

        // Manage State
        result_t RegisterState(const uint32_t stateId, std::shared_ptr<StateBase>& state);
        result_t StateTransitTo(const uint32_t stateId);

        // Manage main loop
        result_t Update();
        result_t Render();

    private:
        std::map<uint32_t, std::shared_ptr<StateBase>>  m_states;
        uint32_t                                        m_currentState;
    };
}