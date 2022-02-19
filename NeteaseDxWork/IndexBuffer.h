#pragma once
#include <d3d11.h>

class ContextDevice;

class IndexBuffer
{
	friend class DeviceContext;
public:
	IndexBuffer() = default;
	~IndexBuffer() = default;

public:
	bool Load(void* indices, UINT indicesSize);
	UINT GetIndexListSize();
	bool Release();

	UINT GetvertexSize() { return indexListSize; }

private:
	UINT indexListSize = 0;

	ID3D11Buffer* pBuffer = nullptr;
	ID3D11InputLayout* pInputLayout = nullptr;
};

