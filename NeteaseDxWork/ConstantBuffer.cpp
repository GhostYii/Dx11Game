#include "ConstantBuffer.h"
#include "RenderSystem.h"
#include "DeviceContext.h"


bool ConstantBuffer::Load(const void* buffer, UINT bufferSize)
{
	if (pBuffer)
		pBuffer->Release();	

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = bufferSize;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = buffer;

	HRESULT res = pRenderSystem->pDevice->CreateBuffer(&bd, &data, &pBuffer);
	if (FAILED(res))
		return false;
	
	return true;
}

void ConstantBuffer::Update(DeviceContext* context, const void* buffer)
{
	context->pDeviceContext->UpdateSubresource(this->pBuffer, NULL, NULL, buffer, NULL, NULL);
}

bool ConstantBuffer::Release()
{
	if (pBuffer)
		pBuffer->Release();
	delete this;
	return true;
}
