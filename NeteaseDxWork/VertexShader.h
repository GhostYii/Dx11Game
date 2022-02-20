#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "GraphicsClasses.h"

class VertexShader
{
	friend class RenderSystem;
	friend class DeviceContext;
public:
	VertexShader(const void* shaderByteCode, size_t byteCodeSize, RenderSystem* rs);
	~VertexShader();

private:
	void Init(const void* shaderByteCode, size_t byteCodeSize);
	void Release();

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader = nullptr;
	RenderSystem* pRenderSystem = nullptr;
};

