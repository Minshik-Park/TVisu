#include "pch.h"
#include "Scene.h"

using namespace TVisu;
using namespace TVisu::Universe;

Scene::~Scene()
{
}

result_t Scene::Update()
{
    return ResultCode::success;
}

result_t Scene::Render()
{
    return ResultCode::success;
}