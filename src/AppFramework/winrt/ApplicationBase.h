#pragma once
#include "Common/TVisu.h"
#include "Common/StepTimer.h"
#include "Universe/World.h"
#include "Graphics/DirectX/DeviceResources.h"
#include "Universe/Objects/Cube.h"

// Renders Direct3D content on the screen.
namespace TVisu::winRT
{
    class ApplicationBase
    {
    public:
        ApplicationBase();
        virtual ~ApplicationBase();

        virtual void CreateRenderers(const std::shared_ptr<DX::DeviceResources>& deviceResources);
        result_t Update();
        result_t Render();

        virtual void OnWindowSizeChanged();
        virtual void OnSuspending();
        virtual void OnResuming();
        virtual void OnDeviceRemoved();

    protected:
        virtual result_t DoUpdate();
        virtual result_t DoRender();

    protected:
        // TODO: Replace with your own content renderers.
        std::unique_ptr<TVisu::Universe::World> m_spWorld;

        // Rendering loop timer.
        DX::StepTimer m_timer;
    };
}