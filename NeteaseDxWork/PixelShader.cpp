#include "PixelShader.h"
#include "RenderSystem.h"

#include <exception>

PixelShader::PixelShader(const void* shaderByteCode, size_t byteCodeSize, RenderSystem* rs) : pRenderSystem(rs)
{
	Init(shaderByteCode, byteCodeSize);
}

PixelShader::~PixelShader()
{
	Release();
}

void PixelShader::Release()
{
	if (pPixelShader)
		pPixelShader->Release();
}

void PixelShader::Init(const void* shaderByteCode, size_t byteCodeSize)
{
	HRESULT res = pRenderSystem->pDevice->CreatePixelShader(shaderByteCode, byteCodeSize, nullptr, &pPixelShader);

	if (FAILED(res))
		throw std::exception("Create PixelShader failed!");
}
