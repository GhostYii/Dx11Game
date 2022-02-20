#pragma once
#include <d3d11.h>
#include "GraphicsClasses.h"

class ConstantBuffer
{
	friend class DeviceContext;
public:
	ConstantBuffer(const void* buffer, UINT bufferSize, RenderSystem* rs);
	~ConstantBuffer();

public:
	void Update(DeviceContext* context, const void* buffer);

private:
	void Load(const void* buffer, UINT bufferSize);	
	void Release();

private:
	ID3D11Buffer* pBuffer = nullptr;
	RenderSystem* pRenderSystem = nullptr;
};

