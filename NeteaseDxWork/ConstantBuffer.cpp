#include "ConstantBuffer.h"
#include "RenderSystem.h"
#include "DeviceContext.h"

#include <exception>


ConstantBuffer::ConstantBuffer(const void* buffer, UINT bufferSize, RenderSystem* rs) :pRenderSystem(rs)
{
	Load(buffer, bufferSize);
}

ConstantBuffer::~ConstantBuffer()
{
	Release();
}

void ConstantBuffer::Load(const void* buffer, UINT bufferSize)
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

	HRESULT res = pRenderSystem->pDevice->CreateBuffer(&bd, &data, pBuffer.GetAddressOf());
	if (FAILED(res))
	{
		throw std::exception("Create ConstantBuffer failed!");
	}
}

void ConstantBuffer::Update(DeviceContextPtr context, const void* buffer)
{
	context->pDeviceContext->UpdateSubresource(pBuffer.Get(), NULL, NULL, buffer, NULL, NULL);
}

void ConstantBuffer::Release()
{
	if (pBuffer)
		pBuffer->Release();
}
