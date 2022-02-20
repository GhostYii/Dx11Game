#pragma once
#include <d3d11.h>

#include "GraphicsClasses.h"

class PixelShader
{
	friend class RenderSystem;
	friend class DeviceContext;

public:
	PixelShader(const void* shaderByteCode, size_t byteCodeSize, RenderSystem* rs);
	~PixelShader();

private:
	void Init(const void* shaderByteCode, size_t byteCodeSize);
	void Release();

private:
	ID3D11PixelShader* pPixelShader = nullptr;
	RenderSystem* pRenderSystem = nullptr;

};

