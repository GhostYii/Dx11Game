#pragma once
#include <d3d11.h>

#include "GraphicsClasses.h"

class VertexBuffer
{
	friend class DeviceContext;
public:
	VertexBuffer(RenderSystem* rs) : pRenderSystem(rs) {}
	~VertexBuffer() = default;

public:
	bool Load(void* vertices, UINT size, UINT sizes, void* shaderByteCode, UINT shaderSizeByte);
	bool Release();

	UINT GetvertexSize() { return vertexSize; }

private:
	UINT vertexSize = 0;
	UINT sizes = 0;

	ID3D11Buffer* pBuffer = nullptr;
	ID3D11InputLayout* pInputLayout = nullptr;

	RenderSystem* pRenderSystem = nullptr;
};

