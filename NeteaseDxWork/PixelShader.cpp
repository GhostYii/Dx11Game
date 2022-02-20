#include "PixelShader.h"
#include "RenderSystem.h"

void PixelShader::Release()
{
	if (pPixelShader)
		pPixelShader->Release();
	delete this;
}

bool PixelShader::Init(const void* shaderByteCode, size_t byteCodeSize)
{
	HRESULT res = pRenderSystem->pDevice->CreatePixelShader(shaderByteCode, byteCodeSize, nullptr, &pPixelShader);

	if (FAILED(res))
		return false;

	return true;
}
