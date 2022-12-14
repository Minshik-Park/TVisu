#pragma once

#include "pch.h"
#include "Graphics\DirectX\DeviceResources.h"
#include "TVisuMain.h"

namespace TVisu
{
    // Main entry point for our app. Connects the app with the Windows shell and handles application lifecycle events.
    struct App : winrt::implements<App, winrt::Windows::ApplicationModel::Core::IFrameworkViewSource, winrt::Windows::ApplicationModel::Core::IFrameworkView>
    {
    public:
        App();

        winrt::Windows::ApplicationModel::Core::IFrameworkView CreateView()
        {
            return *this;
        }

        // IFrameworkView methods.
        virtual void Initialize(winrt::Windows::ApplicationModel::Core::CoreApplicationView const &applicationView);
        virtual void SetWindow(winrt::Windows::UI::Core::CoreWindow const &window);
        virtual void Load(winrt::hstring const &entryPoint);
        virtual void Run();
        virtual void Uninitialize();

    protected:
        // Application lifecycle event handlers.
        void OnActivated(winrt::Windows::ApplicationModel::Core::CoreApplicationView const &applicationView, winrt::Windows::ApplicationModel::Activation::IActivatedEventArgs const &args);
        winrt::fire_and_forget OnSuspending(winrt::Windows::Foundation::IInspectable const &sender, winrt::Windows::ApplicationModel::ISuspendingEventArgs const &args);
        void OnResuming(winrt::Windows::Foundation::IInspectable const &sender, winrt::Windows::Foundation::IInspectable const &args);

        // Window event handlers.
        void OnWindowSizeChanged(winrt::Windows::UI::Core::CoreWindow const &sender, winrt::Windows::UI::Core::WindowSizeChangedEventArgs const &args);
        void OnVisibilityChanged(winrt::Windows::UI::Core::CoreWindow const &sender, winrt::Windows::UI::Core::VisibilityChangedEventArgs const &args);
        void OnWindowClosed(winrt::Windows::UI::Core::CoreWindow const &sender, winrt::Windows::UI::Core::CoreWindowEventArgs const &args);

        // DisplayInformation event handlers.
        void OnDpiChanged(winrt::Windows::Graphics::Display::DisplayInformation const &sender, winrt::Windows::Foundation::IInspectable const &args);
        void OnOrientationChanged(winrt::Windows::Graphics::Display::DisplayInformation const &sender, winrt::Windows::Foundation::IInspectable const &args);
        void OnDisplayContentsInvalidated(winrt::Windows::Graphics::Display::DisplayInformation const &sender, winrt::Windows::Foundation::IInspectable const &args);

    private:
        // Private accessor for m_deviceResources, protects against device removed errors.
        std::shared_ptr<DX::DeviceResources> GetDeviceResources();

        std::shared_ptr<DX::DeviceResources> m_deviceResources;
        std::unique_ptr<TVisuMain> m_main;
        bool m_windowClosed = false;
        bool m_windowVisible = true;
    };
}

