#pragma once
#include <d3d11.h>

class GraphicsEngine;
class DeviceContext;

class PixelShader
{
	friend class GraphicsEngine;
	friend class DeviceContext;

public:
	PixelShader() = default;
	~PixelShader() = default;

public:
	void Release();

private:
	bool Init(const void* shaderByteCode, size_t byteCodeSize);

private:
	ID3D11PixelShader* pPixelShader = nullptr;

};

