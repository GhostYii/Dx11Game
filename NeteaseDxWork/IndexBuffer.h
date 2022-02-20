#pragma once
#include <d3d11.h>
#include <wrl.h>
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

	Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer = nullptr;
	//Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout = nullptr;

	RenderSystem* pRenderSystem = nullptr;

private:
	void Load(void* indices, UINT indicesSize);
	void Release();
};

