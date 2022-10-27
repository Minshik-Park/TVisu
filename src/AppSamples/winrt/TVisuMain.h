#pragma once
#include "AppFramework/winrt/ApplicationBase.h"
#include "Graphics\DirectX\DeviceResources.h"
#include "Universe\Objects\Cube.h"

// Renders Direct3D content on the screen.
namespace TVisu
{
    class TVisuMain : public TVisu::winRT::ApplicationBase
    {
    public:
        TVisuMain();
        virtual result_t Initialize() override;
        virtual void CreateRenderers(const std::shared_ptr<DX::DeviceResources>& deviceResources) override;

        virtual void OnWindowSizeChanged() override;
        virtual void OnSuspending() override;
        virtual void OnResuming() override;
        virtual void OnDeviceRemoved() override;

    protected:
        virtual result_t DoUpdate() override;
        virtual result_t DoRender() override;

    private:
        // TODO: Replace with your own content renderers.
        std::unique_ptr<Cube> m_sceneRenderer;
    };
}