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
        result_t RegisterState(std::shared_ptr<StateBase>& state);
        result_t DeregisterState(const uint32_t id);
        result_t StateTransitTo(const int32_t stateId);

        // Manage main loop
        result_t Update();
        result_t Render();

    private:
        std::map<int32_t, std::shared_ptr<StateBase>>   m_states;
        int32_t                                         m_currentStateIndex = -1;
        std::shared_ptr<StateBase>                      m_spCurrentState;
    };
}