#pragma once
#include <d3d11.h>

#include "GraphicsClasses.h"

class ConstantBuffer
{
	friend class DeviceContext;

public:
	ConstantBuffer(RenderSystem* rs) : pRenderSystem(rs) {}
	~ConstantBuffer() = default;

public:
	bool Load(const void* buffer, UINT bufferSize);
	void Update(DeviceContext* context, const void* buffer);
	bool Release();

private:
	ID3D11Buffer* pBuffer = nullptr;
	RenderSystem* pRenderSystem = nullptr;
};

