#include "pch.h"
#include "World.h"

using namespace TVisu;
using namespace TVisu::Universe;

World::~World()
{
}

result_t World::RegisterState(const uint32_t stateId, std::shared_ptr<StateBase>& state)
{
    return ResultCode::success;
}

result_t World::StateTransitTo(const uint32_t stateId)
{
    return ResultCode::success;
}

result_t World::Update()
{
    return ResultCode::success;
}

result_t World::Render()
{
    return ResultCode::success;
}