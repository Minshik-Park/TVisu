#include "pch.h"
#include "App.h"

using namespace TVisu;
using namespace winrt;
using namespace winrt::Windows;
using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Foundation::Numerics;
using namespace winrt::Windows::Graphics::Display;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Composition;

// The DirectX 12 Application template is documented at https://go.microsoft.com/fwlink/?LinkID=613670&clcid=0x409

// The main function is only used to initialize our IFrameworkView class.
int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    CoreApplication::Run(make<App>());
}

App::App()
{
}

// The first method called when the IFrameworkView is being created.
void App::Initialize(CoreApplicationView const &applicationView)
{
    // Register event handlers for app lifecycle. This example includes Activated, so that we
    // can make the CoreWindow active and start rendering on the window.
    applicationView.Activated({ this, &App::OnActivated });

    CoreApplication::Suspending({ this, &App::OnSuspending });
    CoreApplication::Resuming({ this, &App::OnResuming });
}

// Called when the CoreWindow object is created (or re-created).
void App::SetWindow(CoreWindow const& window)
{
    window.SizeChanged({ this, &App::OnWindowSizeChanged });
    window.VisibilityChanged({ this, &App::OnVisibilityChanged });
    window.Closed({ this, &App::OnWindowClosed });

    DisplayInformation currentDisplayInformation(DisplayInformation::GetForCurrentView());

    currentDisplayInformation.DpiChanged({ this, &App::OnDpiChanged });
    currentDisplayInformation.OrientationChanged({ this, &App::OnOrientationChanged });

    DisplayInformation::DisplayContentsInvalidated({ this, &App::OnDisplayContentsInvalidated });
}

// Initializes scene resources, or loads a previously saved app state.
void App::Load(hstring const &entryPoint)
{
    if (m_main == nullptr)
    {
        m_main = std::unique_ptr<TVisuMain>(new TVisuMain());
    }
}

// This method is called after the window becomes active.
void App::Run()
{
    while (!m_windowClosed)
    {
        if (m_windowVisible)
        {
            CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

            auto commandQueue = GetDeviceResources()->GetCommandQueue();
            PIXBeginEvent(commandQueue, 0, L"Update");
            {
                m_main->Update();
            }
            PIXEndEvent(commandQueue);

            PIXBeginEvent(commandQueue, 0, L"Render");
            {
                if (Succeeded(m_main->Render()))
                {
                    GetDeviceResources()->Present();
                }
            }
            PIXEndEvent(commandQueue);
        }
        else
        {
            CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
        }
    }
}

// Required for IFrameworkView.
// Terminate events do not cause Uninitialize to be called. It will be called if your IFrameworkView
// class is torn down while the app is in the foreground.
void App::Uninitialize()
{
}

// Application lifecycle event handlers.

void App::OnActivated(CoreApplicationView const &applicationView, IActivatedEventArgs const &args)
{
    // Run() won't start until the CoreWindow is activated.
    CoreWindow::GetForCurrentThread().Activate();
}

fire_and_forget App::OnSuspending(IInspectable const &sender, ISuspendingEventArgs const &args)
{
    // Save app state asynchronously after requesting a deferral. Holding a deferral
    // indicates that the application is busy performing suspending operations. Be
    // aware that a deferral may not be held indefinitely. After about five seconds,
    // the app will be forced to exit.
    auto deferral = args.SuspendingOperation().GetDeferral();

    co_await resume_background();

    m_main->OnSuspending();
    deferral.Complete();
}

void App::OnResuming(IInspectable const &sender, IInspectable const &args)
{
    // Restore any data or state that was unloaded on suspend. By default, data
    // and state are persisted when resuming from suspend. Note that this event
    // does not occur if the app was previously terminated.

    m_main->OnResuming();
}

// Window event handlers.

void App::OnWindowSizeChanged(CoreWindow const &sender, WindowSizeChangedEventArgs const &args)
{
    GetDeviceResources()->SetLogicalSize(winrt::Windows::Foundation::Size(sender.Bounds().Width, sender.Bounds().Height));
    m_main->OnWindowSizeChanged();
}

void App::OnVisibilityChanged(CoreWindow const &sender, VisibilityChangedEventArgs const &args)
{
    m_windowVisible = args.Visible();
}

void App::OnWindowClosed(CoreWindow const &sender, CoreWindowEventArgs const &args)
{
    m_windowClosed = true;
}

// DisplayInformation event handlers.

void App::OnDpiChanged(DisplayInformation const &sender, IInspectable const &args)
{
    // Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
    // if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
    // you should always retrieve it using the GetDpi method.
    // See DeviceResources.cpp for more details.
    GetDeviceResources()->SetDpi(sender.LogicalDpi());
    m_main->OnWindowSizeChanged();
}

void App::OnOrientationChanged(DisplayInformation const &sender, IInspectable const &args)
{
    GetDeviceResources()->SetCurrentOrientation(sender.CurrentOrientation());
    m_main->OnWindowSizeChanged();
}

void App::OnDisplayContentsInvalidated(DisplayInformation const &sender, IInspectable const &args)
{
    GetDeviceResources()->ValidateDevice();
}

std::shared_ptr<DX::DeviceResources> App::GetDeviceResources()
{
    if (m_deviceResources != nullptr && m_deviceResources->IsDeviceRemoved())
    {
        // All references to the existing D3D device must be released before a new device
        // can be created.

        m_deviceResources = nullptr;
        m_main->OnDeviceRemoved();

#if defined(_DEBUG)
        winrt::com_ptr<IDXGIDebug1> dxgiDebug;
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
        {
            dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
        }
#endif
    }

    if (m_deviceResources == nullptr)
    {
        m_deviceResources = std::make_shared<DX::DeviceResources>();
        m_deviceResources->SetWindow(CoreWindow::GetForCurrentThread());
        m_main->CreateRenderers(m_deviceResources);
    }
    return m_deviceResources;
}
