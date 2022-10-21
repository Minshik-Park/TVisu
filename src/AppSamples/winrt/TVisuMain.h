#pragma once

#include "Common\StepTimer.h"
#include "Graphics\DirectX\DeviceResources.h"
#include "World\Objects\Cube.h"

// Renders Direct3D content on the screen.
namespace TVisu
{
    class TVisuMain
    {
    public:
        TVisuMain();
        void CreateRenderers(const std::shared_ptr<DX::DeviceResources>& deviceResources);
        void Update();
        bool Render();

        void OnWindowSizeChanged();
        void OnSuspending();
        void OnResuming();
        void OnDeviceRemoved();

    private:
        // TODO: Replace with your own content renderers.
        std::unique_ptr<Cube> m_sceneRenderer;

        // Rendering loop timer.
        DX::StepTimer m_timer;
    };
}