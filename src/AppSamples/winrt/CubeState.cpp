#include "pch.h"
#include "CubeState.h"

using namespace TVisu;

CubeState::CubeState(const uint32_t id) :
    StateBase(id)
{
}

CubeState::~CubeState()
{
}

result_t CubeState::Enter()
{
    result_t result = ResultCode::success;

    if (m_spScene == nullptr)
    {
        m_spScene = std::make_shared<TVisu::Universe::Scene>();
    }

    IF_FAILED_EXIT(m_spScene->Initialize());

Exit:
    return result;
}

result_t CubeState::Exit()
{
    return ResultCode::success;
}

result_t CubeState::Update()
{
    result_t result = ResultCode::success;

    IF_FAILED_EXIT(m_spScene->Update());

Exit:
    return result;
}

result_t CubeState::Render()
{
    result_t result = ResultCode::success;

    IF_FAILED_EXIT(m_spScene->Render());

Exit:
    return result;
}
