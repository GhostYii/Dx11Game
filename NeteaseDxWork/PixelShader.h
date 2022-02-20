#pragma once
#include <d3d11.h>

#include "GraphicsClasses.h"

class PixelShader
{
	friend class RenderSystem;
	friend class DeviceContext;

public:
	PixelShader(RenderSystem* rs) : pRenderSystem(rs) {}
	~PixelShader() = default;

public:
	void Release();

private:
	bool Init(const void* shaderByteCode, size_t byteCodeSize);

private:
	ID3D11PixelShader* pPixelShader = nullptr;
	RenderSystem* pRenderSystem = nullptr;

};

