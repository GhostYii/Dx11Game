#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "GraphicsClasses.h"

class VertexBuffer
{
	friend class DeviceContext;
public:
	VertexBuffer(void* vertices, UINT size, UINT sizes, void* shaderByteCode, UINT shaderSizeByte, RenderSystem* rs);
	~VertexBuffer();

public:
	UINT GetvertexSize() { return vertexSize; }

private:
	UINT vertexSize = 0;
	UINT sizes = 0;

	Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout = nullptr;

	RenderSystem* pRenderSystem = nullptr;

private:
	void Load(void* vertices, UINT size, UINT sizes, void* shaderByteCode, UINT shaderSizeByte);
	void Release();
};

