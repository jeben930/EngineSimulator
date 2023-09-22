#include "pch.h"
#include "Sample3DSceneRenderer.h"

using namespace EngineSimulator;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources, const std::shared_ptr<CMBDataset>& cmbDataset) :
    m_deviceResources(deviceResources), m_cmbDataset(cmbDataset)
{
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
}

// Called when the device-dependent resources need to be created.
void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
    // TODO: Initialize device-dependent resources.
    auto device = m_deviceResources->GetD3DDevice();

    // Create a primitive batch for rendering points.
    m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(device);

    // Create a root signature.
    CD3DX12_ROOT_PARAMETER rootParameters[1];
    CD3DX12_DESCRIPTOR_RANGE descriptorRanges[1];
    descriptorRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    rootParameters[0].InitAsDescriptorTable(1, &descriptorRanges[0], D3D12_SHADER_VISIBILITY_PIXEL);

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    Microsoft::WRL::ComPtr<ID3DBlob> signature;
    Microsoft::WRL::ComPtr<ID3DBlob> error;
    DX::ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
    DX::ThrowIfFailed(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));

    // Create a pipeline state object.
    Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
    Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;

    // Load shaders asynchronously.
    auto loadVSTask = DX::ReadDataAsync(L"VertexShader.cso");
    auto loadPSTask = DX::ReadDataAsync(L"PixelShader.cso");

    // After the vertex shader file is loaded, create the shader and input layout.
    auto createVSTask = loadVSTask.then([this, &vertexShader](const std::vector<byte>& fileData) {
        DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateVertexShader(fileData.data(), fileData.size(), nullptr, &vertexShader));

        const D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };

        CD3DX12_BLEND_DESC blendDesc(D3D12_DEFAULT);
        CD3DX12_RASTERIZER_DESC rasterizerDesc(D3D12_DEFAULT);
        CD3DX12_DEPTH_STENCIL_DESC depthStencilDesc(D3D12_DEFAULT);

        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
        psoDesc.pRootSignature = m_rootSignature.Get();
        psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
        psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
        psoDesc.RasterizerState = rasterizerDesc;
        psoDesc.BlendState = blendDesc;
        psoDesc.DepthStencilState = depthStencilDesc;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = m_deviceResources->GetBackBufferFormat();
        psoDesc.SampleDesc.Count = 1;
        DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
        });

    // After the pixel shader file is loaded, create the shader and texture.
    auto createPSTask = loadPSTask.then([this, &pixelShader](const std::vector<byte>& fileData) {
        DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreatePixelShader(fileData.data(), fileData.size(), nullptr, &pixelShader));
        });

    // Wait for both shaders to finish loading.
    auto createShadersTask = (createVSTask && createPSTask).then([this]() {
        // TODO: Initialize any additional device-dependent resources required for rendering.
        // For example, you can create a vertex buffer and index buffer to represent the particles in the simulation.
        // You can use the data in the CMBDataset object to populate the vertex buffer and index buffer.
        // You can also create any additional textures or resources required for rendering.
        });

    createShadersTask.wait();
}

// Called when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
    // TODO: Initialize window-size dependent resources.
    auto size = m_deviceResources->GetOutputSize();
    float aspectRatio = static_cast<float>(size.right) / static_cast<float>(size.bottom);

    // TODO: Update any projection matrices or viewport sizes required for rendering.
}

// Called once per frame, updates the scene state.
void Sample3DSceneRenderer::Update(DX::StepTimer const& timer)
{
    // TODO: Update any scene objects or animations.
    m_timer = timer;
}

// Renders one frame using the DirectX 12 API.
bool Sample3DSceneRenderer::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return false;
    }

    auto commandList = m_deviceResources->GetCommandList();
    auto renderTarget = m_deviceResources->GetRenderTargetView();

    // Record commands.
    commandList->OMSetRenderTargets(1, &renderTarget, FALSE, nullptr);
    commandList->ClearRenderTargetView(renderTarget, DirectX::Colors::CornflowerBlue, 0, nullptr);

    // Set pipeline state and root signature.
    commandList->SetPipelineState(m_pipelineState.Get());
    commandList->SetGraphicsRootSignature(m_rootSignature.Get());

    // TODO: Render the contents of the CMBDataset object.
    // You can access the data in the CMBDataset object using its public methods and properties.
    // You can use the DirectX 12 API to render the data to the screen.
    // For example, you can create a vertex buffer and index buffer to represent the particles in the simulation,
    // and use a shader to render the particles as points or spheres.

    // Render the primitive batch.
    m_batch->Begin(commandList.Get());
    m_batch->Draw(DirectX::VertexPositionColor(DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::Colors::White));
    m_batch->End();

    // Indicate that the back buffer will now be used to present.
    commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetRenderTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    return true;
}

// Releases device-dependent resources.
void Sample3DSceneRenderer::ReleaseDeviceDependentResources()
{
    // TODO: Release device-dependent resources.
    m_batch.reset();
    m_rootSignature.Reset();
    m_pipelineState.Reset();
    m_vertexBuffer.Reset();
}