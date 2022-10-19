#pragma once

namespace DX
{
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch Win32 API errors.
            winrt::throw_hresult(hr);
        }
    }

    // Function that reads from a binary file asynchronously.
    inline winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Storage::Streams::IBuffer> ReadDataAsync(const std::wstring& filename)
    {
        auto folder = winrt::Windows::ApplicationModel::Package::Current().InstalledLocation();

        winrt::Windows::Storage::StorageFile file{ co_await folder.GetFileAsync(winrt::hstring(filename.c_str())) };
        co_return co_await winrt::Windows::Storage::FileIO::ReadBufferAsync(file);
    }

    // Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
    inline float ConvertDipsToPixels(float dips, float dpi)
    {
        static const float dipsPerInch = 96.0f;
        return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
    }

    // Assign a name to the object to aid with debugging.
#if defined(_DEBUG)
    inline void SetName(ID3D12Object* pObject, LPCWSTR name)
    {
        pObject->SetName(name);
    }
#else
    inline void SetName(ID3D12Object*, LPCWSTR)
    {
    }
#endif
}

// Naming helper function for ComPtr<T>.
// Assigns the name of the variable as the name of the object.
#define NAME_D3D12_OBJECT(x) DX::SetName(x.get(), L#x)
