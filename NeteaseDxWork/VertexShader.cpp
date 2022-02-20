#include "VertexShader.h"
#include "RenderSystem.h"

#include <exception>

void VertexShader::Release()
{
	pVertexShader->Release();
}

VertexShader::VertexShader(const void* shaderByteCode, size_t byteCodeSize, RenderSystem* rs) : pRenderSystem(rs)
{
	Init(shaderByteCode, byteCodeSize);
}

VertexShader::~VertexShader()
{
	Release();
}

void VertexShader::Init(const void* shaderByteCode, size_t byteCodeSize)
{
	HRESULT res = pRenderSystem->pDevice->CreateVertexShader(shaderByteCode, byteCodeSize, nullptr, pVertexShader.GetAddressOf());
	if (FAILED(res))
		throw std::exception("Create VertexShader failed!");
}
