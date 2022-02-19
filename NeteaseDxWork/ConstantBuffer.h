#pragma once
#include <d3d11.h>

class DeviceContext;

class ConstantBuffer
{
	friend class DeviceContext;

public:
	ConstantBuffer() = default;
	~ConstantBuffer() = default;

public:
	bool Load(const void* buffer, UINT bufferSize);
	void Update(DeviceContext* context, const void* buffer);
	bool Release();

private:
	ID3D11Buffer* pBuffer = nullptr;
};

