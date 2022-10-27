#include "pch.h"
#include "TVisuMain.h"
#include "CubeState.h"
#include "Graphics\DirectX\DirectXHelper.h"

using namespace TVisu;

// The DirectX 12 Application template is documented at https://go.microsoft.com/fwlink/?LinkID=613670&clcid=0x409

// Loads and initializes application assets when the application is loaded.
TVisuMain::TVisuMain()
{
    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
}

result_t TVisuMain::Initialize()
{
    result_t result = ResultCode::success;
    std::shared_ptr<TVisu::Universe::StateBase> spCubeState;

    IF_FAILED_EXIT(ApplicationBase::Initialize());

    spCubeState = std::make_shared<CubeState>(0);

    IF_FAILED_EXIT(m_spWorld->RegisterState(spCubeState));

Exit:
    return result;
}


// Creates and initializes the renderers.
void TVisuMain::CreateRenderers(const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
    // TODO: Replace this with your app's content initialization.
    m_sceneRenderer = std::unique_ptr<Cube>(new Cube(deviceResources));

    OnWindowSizeChanged();
}

// Updates the application state once per frame.
result_t TVisuMain::DoUpdate()
{
    result_t result = ResultCode::success;

    IF_FAILED_EXIT(ApplicationBase::DoUpdate());

    // TODO: Replace this with your app's content update functions.
    m_sceneRenderer->Update(m_timer);

Exit:
    return result;
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
result_t TVisuMain::DoRender()
{
    result_t result = ResultCode::success;

    IF_FAILED_EXIT(ApplicationBase::DoRender());

    // Render the scene objects.
    // TODO: Replace this with your app's content rendering functions.
    if (!m_sceneRenderer->Render())
    {
        result = ResultCode::failed;
    }

Exit:
    return result;
}

// Updates application state when the window's size changes (e.g. device orientation change)
void TVisuMain::OnWindowSizeChanged()
{
    // TODO: Replace this with the size-dependent initialization of your app's content.
    m_sceneRenderer->CreateWindowSizeDependentResources();
}

// Notifies the app that it is being suspended.
void TVisuMain::OnSuspending()
{
    // TODO: Replace this with your app's suspending logic.

    // Process lifetime management may terminate suspended apps at any time, so it is
    // good practice to save any state that will allow the app to restart where it left off.

    m_sceneRenderer->SaveState();

    // If your application uses video memory allocations that are easy to re-create,
    // consider releasing that memory to make it available to other applications.
}

// Notifes the app that it is no longer suspended.
void TVisuMain::OnResuming()
{
    // TODO: Replace this with your app's resuming logic.
}

// Notifies renderers that device resources need to be released.
void TVisuMain::OnDeviceRemoved()
{
    // TODO: Save any necessary application or renderer state and release the renderer
    // and its resources which are no longer valid.
    m_sceneRenderer->SaveState();
    m_sceneRenderer = nullptr;
}
