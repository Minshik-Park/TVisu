#include "pch.h"
#include "ApplicationBase.h"
#include "Graphics\DirectX\DirectXHelper.h"

using namespace TVisu;
using namespace TVisu::winRT;
using namespace TVisu::Universe;

// The DirectX 12 Application template is documented at https://go.microsoft.com/fwlink/?LinkID=613670&clcid=0x409

// Loads and initializes application assets when the application is loaded.
ApplicationBase::ApplicationBase()
{
    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
}

ApplicationBase::~ApplicationBase()
{
}

result_t ApplicationBase::Initialize()
{
    result_t result = ResultCode::success;
    IF_FALSE_EXIT(m_spWorld == nullptr, ResultCode::alreadyInitialized);

    // Create world and register states
    m_spWorld = std::make_unique<World>();
    IF_NULL_EXIT(m_spWorld, ResultCode::outOfMemory);

Exit:
    return result;
}

// Creates and initializes the renderers.
void ApplicationBase::CreateRenderers(const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
}

// Updates the application state once per frame.
result_t ApplicationBase::Update()
{
    // Update scene objects.
    return m_timer.Tick([&]() -> result_t
    {
        return DoUpdate();
    });
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
result_t ApplicationBase::Render()
{
    result_t result = ResultCode::success;

    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return ResultCode::success;
    }

    IF_FAILED_EXIT(DoRender());

Exit:
    return result;
}

// Updates application state when the window's size changes (e.g. device orientation change)
void ApplicationBase::OnWindowSizeChanged()
{
}

// Notifies the app that it is being suspended.
void ApplicationBase::OnSuspending()
{
}

// Notifes the app that it is no longer suspended.
void ApplicationBase::OnResuming()
{
}

// Notifies renderers that device resources need to be released.
void ApplicationBase::OnDeviceRemoved()
{
}

result_t ApplicationBase::DoUpdate()
{
    result_t result = ResultCode::success;
    IF_FAILED_THROW(m_spWorld->Update(), "Failed to update world");

    return result;
}

result_t ApplicationBase::DoRender()
{
    result_t result = ResultCode::success;

    // Render the scene objects.
    // TODO: Replace this with your app's content rendering functions.
    IF_FAILED_EXIT(m_spWorld->Render());

Exit:
    return result;
}
