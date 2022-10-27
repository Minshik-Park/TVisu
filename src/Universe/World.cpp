#include "pch.h"
#include "World.h"

using namespace TVisu;
using namespace TVisu::Universe;

World::~World()
{
}

result_t World::RegisterState(std::shared_ptr<StateBase>& state)
{
    result_t result = ResultCode::success;
    IF_FALSE_EXIT(m_states.find(state->Id()) == m_states.end(), ResultCode::alreadyInitialized);

    m_states[state->Id()] = state;

Exit:
    return result;
}

result_t World::DeregisterState(const uint32_t id)
{
    result_t result = ResultCode::success;
    IF_FALSE_EXIT(m_currentStateIndex != id, ResultCode::invalidState); // Can't remove current state

    {
        auto it = m_states.find(id);

        if (it != m_states.end())
        {
            m_states.erase(it);
        }
    }

Exit:
    return result;

}

result_t World::StateTransitTo(const int32_t stateId)
{
    result_t result = ResultCode::success;
    auto targetStateIt = m_states.find(stateId);    // Try to find new state object

    // Failed out if new state is not registered yet
    IF_FALSE_EXIT(targetStateIt != m_states.end(), ResultCode::invalidParameter);

    // If world has current state running. Exit from it.
    if (m_spCurrentState)
    {
        IF_FAILED_EXIT(m_spCurrentState->Exit());
    }

    // Transit to target state
    m_spCurrentState = targetStateIt->second;
    m_currentStateIndex = stateId;

    // Enter the new current state
    IF_FAILED_EXIT(m_spCurrentState->Enter());

Exit:
    return result;
}

result_t World::Update()
{
    result_t result = ResultCode::success;
    IF_NULL_EXIT(m_spCurrentState, ResultCode::invalidState);

    IF_FAILED_EXIT(m_spCurrentState->Update());

Exit:
    return result;
}

result_t World::Render()
{
    result_t result = ResultCode::success;
    IF_NULL_EXIT(m_spCurrentState, ResultCode::invalidState);

    IF_FAILED_EXIT(m_spCurrentState->Render());

Exit:
    return result;
}