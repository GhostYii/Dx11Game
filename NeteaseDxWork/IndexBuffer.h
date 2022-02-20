#pragma once
#include <d3d11.h>

#include "GraphicsClasses.h"

class IndexBuffer
{
	friend class DeviceContext;
public:
	IndexBuffer(void* indices, UINT indicesSize, RenderSystem* rs);
	~IndexBuffer();

public:
	UINT GetIndexListSize();
	UINT GetvertexSize() { return indexListSize; }

private:
	UINT indexListSize = 0;

	ID3D11Buffer* pBuffer = nullptr;
	ID3D11InputLayout* pInputLayout = nullptr;

	RenderSystem* pRenderSystem = nullptr;

private:
	void Load(void* indices, UINT indicesSize);
	void Release();
};

