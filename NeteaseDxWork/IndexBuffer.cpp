#include "IndexBuffer.h"
#include "GraphicsEngine.h"

bool IndexBuffer::Load(void* indices, UINT indicesSize)
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

	HRESULT res = GraphicsEngine::GetInstance()->pDevice->CreateBuffer(&bd, &data, &pBuffer);

	if (FAILED(res))
		return false;

	return false;
}

UINT IndexBuffer::GetIndexListSize()
{
	return indexListSize;
}

bool IndexBuffer::Release()
{
	pBuffer->Release();
	delete this;
	return true;
}
