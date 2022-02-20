#include "IndexBuffer.h"
#include "RenderSystem.h"
#include <exception>

IndexBuffer::IndexBuffer(void* indices, UINT indicesSize, RenderSystem* rs) : pRenderSystem(rs)
{
	Load(indices, indicesSize);
}

IndexBuffer::~IndexBuffer()
{
	Release();
}

void IndexBuffer::Load(void* indices, UINT indicesSize)
{
	if (pBuffer)
		pBuffer->Release();

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UINT) * indicesSize;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = indices;

	indexListSize = indicesSize;

	HRESULT res = pRenderSystem->pDevice->CreateBuffer(&bd, &data, pBuffer.GetAddressOf());

	if (FAILED(res))
		throw std::exception("Create IndexBuffer failed!");
}

UINT IndexBuffer::GetIndexListSize()
{
	return indexListSize;
}

void IndexBuffer::Release()
{
	pBuffer->Release();
}
