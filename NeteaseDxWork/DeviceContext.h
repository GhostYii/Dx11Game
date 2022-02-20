#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "GraphicsClasses.h"
#include "ResourceClasses.h"

class DeviceContext
{
	friend class ConstantBuffer;
public:
	DeviceContext(ID3D11DeviceContext* pDeviceContext, RenderSystem* rs);
	~DeviceContext();

public:
	void ClearRenderTargetColor(SwapChainPtr swapChain, float r, float g, float b, float a);
	void SetVertexBuffer(VertexBufferPtr pBuffer);
	void SetIndexBuffer(IndexBufferPtr pBuffer);

	void DrawTriangleList(UINT vertexSize, UINT startIndex);
	void DrawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation);
	void SetViewportSize(UINT width, UINT height);

	void SetVertexShader(VertexShaderPtr shader);
	void SetPixelShader(PixelShaderPtr shader);

	void VSSetConstantBuffer(ConstantBufferPtr cBuffer);
	void PSSetConstantBuffer(ConstantBufferPtr cBuffer);

	void VSSetTexture(TexturePtr texture);
	void PSSetTexture(TexturePtr texture);
private:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
	RenderSystem* pRenderSystem = nullptr;
};

