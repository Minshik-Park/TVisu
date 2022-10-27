#pragma once
#include "Common/TVisu.h"
#include "Graphics/RenderingContext.h"
#include <d3d12.h>
#include "ShaderStructures.h"

namespace TVisu::Graphics
{
    class RenderingContextDX12 : public RenderingContext
    {
    public:
        RenderingContextDX12() = default;
        virtual ~RenderingContextDX12();

        result_t CreateRootSignature(winrt::com_ptr<ID3D12Device> &spD3DDevice);
        result_t CreatePipelineState(
            winrt::com_ptr<ID3D12Device>& spD3DDevice,
            DXGI_FORMAT backBufferFormat,
            DXGI_FORMAT depthBufferFormat,
            winrt::Windows::Storage::Streams::IBuffer &vertexShader,    // ToDo: using std::vector<uint8_t>&
            winrt::Windows::Storage::Streams::IBuffer &pixelShader);    // ToDo: using std::vector<uint8_t>&
        result_t CreateCommandList(
            winrt::com_ptr<ID3D12Device>& spD3DDevice,
            winrt::com_ptr<ID3D12CommandAllocator> spCommandAllocator);
        result_t CreateVertexBuffer(
            winrt::com_ptr<ID3D12Device>& spD3DDevice,
            std::vector<VertexPositionColor>& vertexList);
        result_t CreateIndexBuffer(
            winrt::com_ptr<ID3D12Device>& spD3DDevice,
            std::vector<unsigned short>& indexList);

    private:
        winrt::com_ptr<ID3D12GraphicsCommandList>   m_spCommandList;
        winrt::com_ptr<ID3D12RootSignature>         m_spRootSignature;
        winrt::com_ptr<ID3D12PipelineState>         m_spPipelineState;
        winrt::com_ptr<ID3D12DescriptorHeap>        m_spCbvHeap;
        winrt::com_ptr<ID3D12Resource>              m_spVertexBuffer;
        winrt::com_ptr<ID3D12Resource>              m_spIndexBuffer;
        winrt::com_ptr<ID3D12Resource>              m_spConstantBuffer;
        ModelViewProjectionConstantBuffer           m_constantBufferData;
        UINT8* m_mappedConstantBuffer;
        UINT                                        m_cbvDescriptorSize;
        D3D12_RECT                                  m_scissorRect;
        D3D12_VERTEX_BUFFER_VIEW                    m_vertexBufferView;
        D3D12_INDEX_BUFFER_VIEW                     m_indexBufferView;
    };
}