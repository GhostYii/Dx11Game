#pragma once
#include <d3d11.h>

class GraphicsEngine;
class DeviceContext;

class VertexShader
{
	friend class GraphicsEngine;
	friend class DeviceContext;
public:
	VertexShader() = default;
	~VertexShader() = default;

public:	
	void Release();

private:
	bool Init(const void* shaderByteCode, size_t byteCodeSize);

private:
	ID3D11VertexShader* pVertexShader = nullptr;
};

