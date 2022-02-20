#include "VertexShader.h"
#include "RenderSystem.h"

void VertexShader::Release()
{
	pVertexShader->Release();
	delete this;
}

bool VertexShader::Init(const void* shaderByteCode, size_t byteCodeSize)
{
	HRESULT res = pRenderSystem->pDevice->CreateVertexShader(shaderByteCode, byteCodeSize, nullptr, &pVertexShader);
	if (FAILED(res))
		return false;

	return true;
}
