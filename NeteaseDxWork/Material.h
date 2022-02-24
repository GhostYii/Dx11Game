#pragma once
#include "PinelineStruct.h"
#include <vector>

enum D3D11_CULL_MODE;
class GraphicsEngine;

class Material
{
	friend class GraphicsEngine;
public:
	Material(const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath);
	Material(const Material& other);
	~Material() = default;
	
public:
	D3D11_CULL_MODE GetCullMode() { return cullMode; }

	void AddTexture(const TexturePtr& texture);
	void EraseTexture(UINT index);

	void SetCBuffer(void* cbuff, UINT bufferSize);
	void SetCullMode(D3D11_CULL_MODE mode);

private:
	VertexShaderPtr pVertexShader = nullptr;
	PixelShaderPtr pPixelShader = nullptr;
	ConstantBufferPtr pConstantBuffer = nullptr;

	D3D11_CULL_MODE cullMode = D3D11_CULL_BACK;

	std::vector<TexturePtr> textures;
};

