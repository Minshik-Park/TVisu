#include "pch.h"
#include "RenderingContextDX12.h"
#include "Graphics/DirectX/DirectXHelper.h"

using namespace TVisu;
using namespace TVisu::Graphics;

RenderingContextDX12::~RenderingContextDX12()
{
}

result_t RenderingContextDX12::CreateRootSignature(winrt::com_ptr<ID3D12Device>& spD3DDevice)
{
    result_t result = ResultCode::success;

    CD3DX12_DESCRIPTOR_RANGE range;
    CD3DX12_ROOT_PARAMETER parameter;

    range.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    parameter.InitAsDescriptorTable(1, &range, D3D12_SHADER_VISIBILITY_VERTEX);

    D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | // Only the input assembler stage needs access to the constant buffer.
        D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

    CD3DX12_ROOT_SIGNATURE_DESC descRootSignature;
    descRootSignature.Init(1, &parameter, 0, nullptr, rootSignatureFlags);

    winrt::com_ptr<ID3DBlob> spSignature;
    winrt::com_ptr<ID3DBlob> spError;
    IF_FAILED_EXIT(D3D12SerializeRootSignature(&descRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, spSignature.put(), spError.put()));
    IF_FAILED_EXIT(spD3DDevice->CreateRootSignature(0, spSignature->GetBufferPointer(), spSignature->GetBufferSize(), IID_PPV_ARGS(m_spRootSignature.put())));

    NAME_D3D12_OBJECT(m_spRootSignature);

Exit:
    return result;
}

result_t RenderingContextDX12::CreatePipelineState(
    winrt::com_ptr<ID3D12Device>& spD3DDevice,
    DXGI_FORMAT backBufferFormat,
    DXGI_FORMAT depthBufferFormat,
    winrt::Windows::Storage::Streams::IBuffer& vertexShader,    // ToDo: using std::vector<uint8_t>&
    winrt::Windows::Storage::Streams::IBuffer& pixelShader)     // ToDo: using std::vector<uint8_t>&
{
    result_t result = ResultCode::success;

    static const D3D12_INPUT_ELEMENT_DESC inputLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC state = {};
    state.InputLayout           = { inputLayout, _countof(inputLayout) };
    state.pRootSignature        = m_spRootSignature.get();
    state.VS                    = CD3DX12_SHADER_BYTECODE(vertexShader.data(), vertexShader.Length());
    state.PS                    = CD3DX12_SHADER_BYTECODE(pixelShader.data(), pixelShader.Length());
    state.RasterizerState       = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    state.BlendState            = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    state.DepthStencilState     = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    state.SampleMask            = UINT_MAX;
    state.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    state.NumRenderTargets      = 1;
    state.RTVFormats[0]         = backBufferFormat;
    state.DSVFormat             = depthBufferFormat;
    state.SampleDesc.Count      = 1;

    IF_FAILED_EXIT(spD3DDevice->CreateGraphicsPipelineState(&state, IID_PPV_ARGS(m_spPipelineState.put())));

Exit:
    return result;
}

result_t RenderingContextDX12::CreateCommandList(
    winrt::com_ptr<ID3D12Device>& spD3DDevice,
    winrt::com_ptr<ID3D12CommandAllocator> spCommandAllocator)
{
    result_t result = ResultCode::success;

    // Create a command list.
    IF_FAILED_EXIT(spD3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, spCommandAllocator.get(), m_spPipelineState.get(), IID_PPV_ARGS(m_spCommandList.put())));
    NAME_D3D12_OBJECT(m_spCommandList);

Exit:
    return result;
}

result_t RenderingContextDX12::CreateVertexBuffer(
    winrt::com_ptr<ID3D12Device>& spD3DDevice,
    std::vector<VertexPositionColor>& vertexList)
{
    result_t result = ResultCode::success;
    const UINT vertexBufferSize = (UINT)(sizeof(VertexPositionColor) * vertexList.size());
    winrt::com_ptr<ID3D12Resource> spVertexBufferUpload;
    CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC vertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
    CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);

    // Create the vertex buffer resource in the GPU's default heap and copy vertex data into it using the upload heap.
    // The upload resource must not be released until after the GPU has finished using it.

    IF_FAILED_EXIT(spD3DDevice->CreateCommittedResource(
        &defaultHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &vertexBufferDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(m_spVertexBuffer.put())));

    IF_FAILED_EXIT(spD3DDevice->CreateCommittedResource(
        &uploadHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &vertexBufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(spVertexBufferUpload.put())));

    NAME_D3D12_OBJECT(m_spVertexBuffer);

    // Upload the vertex buffer to the GPU.
    {
        D3D12_SUBRESOURCE_DATA vertexData = {};
        vertexData.pData = reinterpret_cast<BYTE*>(vertexList.data());
        vertexData.RowPitch = vertexBufferSize;
        vertexData.SlicePitch = vertexData.RowPitch;

        UpdateSubresources(m_spCommandList.get(), m_spVertexBuffer.get(), spVertexBufferUpload.get(), 0, 0, 1, &vertexData);

        CD3DX12_RESOURCE_BARRIER vertexBufferResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_spVertexBuffer.get(),
                                                                                                    D3D12_RESOURCE_STATE_COPY_DEST,
                                                                                                    D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
        m_spCommandList->ResourceBarrier(1, &vertexBufferResourceBarrier);
    }

    // Create vertex buffer views.
    m_vertexBufferView.BufferLocation = m_spVertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes  = sizeof(VertexPositionColor);
    m_vertexBufferView.SizeInBytes    = vertexBufferSize;

Exit:
    return result;
}

result_t RenderingContextDX12::CreateIndexBuffer(
    winrt::com_ptr<ID3D12Device>& spD3DDevice,
    std::vector<unsigned short>& indexList)
{
    result_t result = ResultCode::success;
    winrt::com_ptr<ID3D12Resource> spIndexBufferUpload;
    const UINT indexBufferSize = (UINT)(sizeof(short) * indexList.size());
    CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);

    // Create the index buffer resource in the GPU's default heap and copy index data into it using the upload heap.
    // The upload resource must not be released until after the GPU has finished using it.

    CD3DX12_RESOURCE_DESC indexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);
    IF_FAILED_EXIT(spD3DDevice->CreateCommittedResource(
        &defaultHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &indexBufferDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(m_spIndexBuffer.put())));

    IF_FAILED_EXIT(spD3DDevice->CreateCommittedResource(
        &uploadHeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &indexBufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(spIndexBufferUpload.put())));

    NAME_D3D12_OBJECT(m_spIndexBuffer);

    // Upload the index buffer to the GPU.
    {
        D3D12_SUBRESOURCE_DATA indexData = {};
        indexData.pData = reinterpret_cast<BYTE*>(indexList.data());
        indexData.RowPitch = indexBufferSize;
        indexData.SlicePitch = indexData.RowPitch;

        UpdateSubresources(m_spCommandList.get(), m_spIndexBuffer.get(), spIndexBufferUpload.get(), 0, 0, 1, &indexData);

        CD3DX12_RESOURCE_BARRIER indexBufferResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_spIndexBuffer.get(),
                                                                                                   D3D12_RESOURCE_STATE_COPY_DEST,
                                                                                                   D3D12_RESOURCE_STATE_INDEX_BUFFER);
        m_spCommandList->ResourceBarrier(1, &indexBufferResourceBarrier);
    }

    // Create index buffer views.
    m_indexBufferView.BufferLocation = m_spIndexBuffer->GetGPUVirtualAddress();
    m_indexBufferView.SizeInBytes = indexBufferSize;
    m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;

Exit:
    return result;
}
