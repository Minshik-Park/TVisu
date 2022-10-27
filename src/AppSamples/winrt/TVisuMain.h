﻿#pragma once
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
        virtual void CreateRenderers(const std::shared_ptr<DX::DeviceResources>& deviceResources);

        virtual void OnWindowSizeChanged();
        virtual void OnSuspending();
        virtual void OnResuming();
        virtual void OnDeviceRemoved();

    protected:
        virtual result_t DoUpdate();
        virtual result_t DoRender();

    private:
        // TODO: Replace with your own content renderers.
        std::unique_ptr<Cube> m_sceneRenderer;
    };
}