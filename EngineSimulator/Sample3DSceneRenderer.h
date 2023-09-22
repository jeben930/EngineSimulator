#pragma once
#pragma once

#include "DeviceResources.h"
#include "ShaderStructures.h"
#include "StepTimer.h"
#include "CMBDataset.h"

namespace EngineSimulator
{
    // Renders the scene using the DirectX 12 API.
    class Sample3DSceneRenderer
    {
    public:
        Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources, const std::shared_ptr<CMBDataset>& cmbDataset);
        void CreateDeviceDependentResources();
        void CreateWindowSizeDependentResources();
        void ReleaseDeviceDependentResources();
        void Update(DX::StepTimer const& timer);
        bool Render();

    private:
        // Cached pointer to device resources.
        std::shared_ptr<DX::DeviceResources> m_deviceResources;

        // TODO: Replace with your own content renderers.
        std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;
        Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
        Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
        Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
        D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

        // CMBDataset object to render.
        std::shared_ptr<CMBDataset> m_cmbDataset;

        // Rendering loop timer.
        DX::StepTimer m_timer;
    };
}