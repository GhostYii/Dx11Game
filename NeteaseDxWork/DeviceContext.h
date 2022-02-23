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
	void SetVertexBuffer(const VertexBufferPtr& pBuffer);
	void SetIndexBuffer(const IndexBufferPtr& pBuffer);

	void DrawTriangleList(UINT vertexSize, UINT startIndex);
	void DrawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation);
	void SetViewportSize(UINT width, UINT height);

	void SetVertexShader(const VertexShaderPtr& shader);
	void SetPixelShader(const PixelShaderPtr& shader);

	void VSSetConstantBuffer(const ConstantBufferPtr& cBuffer);
	void PSSetConstantBuffer(const ConstantBufferPtr& cBuffer);

	void VSSetTexture(const TexturePtr* textures, UINT size);
	void PSSetTexture(const TexturePtr* texture, UINT size);
	void PSSetTexture(const TexturePtr& texture);
private:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
	RenderSystem* pRenderSystem = nullptr;
};

