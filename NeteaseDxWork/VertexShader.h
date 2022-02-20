#pragma once
#include <d3d11.h>

#include "GraphicsClasses.h"

class VertexShader
{
	friend class RenderSystem;
	friend class DeviceContext;
public:
	VertexShader(RenderSystem* rs) : pRenderSystem(rs) {}
	~VertexShader() = default;

public:	
	void Release();

private:
	bool Init(const void* shaderByteCode, size_t byteCodeSize);

private:
	ID3D11VertexShader* pVertexShader = nullptr;
	RenderSystem* pRenderSystem = nullptr;
};

