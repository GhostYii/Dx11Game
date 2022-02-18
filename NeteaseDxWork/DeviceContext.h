#pragma once
#include <d3d11.h>

class SwapChain;
class VertexBuffer;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* pDeviceContext);
	~DeviceContext() = default;

public:
	bool Release();
	bool ClearRenderTargetColor(SwapChain* swapChain, float r, float g, float b, float a);
	bool SetVertexBuffer(VertexBuffer* pBuffer);

	void DrawTriangleList(UINT vertexSize, UINT startIndex);
	void SetViewportSize(UINT width, UINT height);

private:
	ID3D11DeviceContext* pDeviceContext = nullptr;
};

