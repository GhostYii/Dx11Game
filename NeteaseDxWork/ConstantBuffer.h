#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "GraphicsClasses.h"

class ConstantBuffer
{
	friend class DeviceContext;
public:
	ConstantBuffer(const void* buffer, UINT bufferSize, RenderSystem* rs);
	~ConstantBuffer();

public:
	void Update(DeviceContextPtr context, const void* buffer);

private:
	void Load(const void* buffer, UINT bufferSize);	
	void Release();

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer = nullptr;
	RenderSystem* pRenderSystem = nullptr;
};

