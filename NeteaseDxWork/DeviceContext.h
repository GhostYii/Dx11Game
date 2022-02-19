#pragma once
#include <d3d11.h>

class SwapChain;
class VertexBuffer;
class ConstantBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;

class DeviceContext
{
	friend class ConstantBuffer;
public:
	DeviceContext(ID3D11DeviceContext* pDeviceContext);
	~DeviceContext() = default;

public:
	bool Release();
	void ClearRenderTargetColor(SwapChain* swapChain, float r, float g, float b, float a);
	void SetVertexBuffer(VertexBuffer* pBuffer);
	void SetIndexBuffer(IndexBuffer* pBuffer);

	void DrawTriangleList(UINT vertexSize, UINT startIndex);
	void DrawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation);
	void SetViewportSize(UINT width, UINT height);

	void SetVertexShader(VertexShader* shader);
	void SetPixelShader(PixelShader* shader);

	void VSSetConstantBuffer(/*VertexShader* shader,*/ ConstantBuffer* cBuffer);
	void PSSetConstantBuffer(/*PixelShader* shader,*/ ConstantBuffer* cBuffer);
private:
	ID3D11DeviceContext* pDeviceContext = nullptr;
};

