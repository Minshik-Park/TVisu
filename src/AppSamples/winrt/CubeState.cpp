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
    return ResultCode::success;
}

result_t CubeState::Exit()
{
    return ResultCode::success;
}

result_t CubeState::Update()
{
    return ResultCode::success;
}

result_t CubeState::Render()
{
    return ResultCode::success;
}
