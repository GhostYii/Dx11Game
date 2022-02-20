#include "VertexBuffer.h"
#include "RenderSystem.h"

#include <exception>

VertexBuffer::VertexBuffer(void* vertices, UINT size, UINT sizes, void* shaderByteCode, UINT shaderSizeByte, RenderSystem* rs)
	: pRenderSystem(rs)
{
	Load(vertices, size, sizes, shaderByteCode, shaderSizeByte);
}

VertexBuffer::~VertexBuffer()
{
	Release();
}

void VertexBuffer::Load(void* vertices, UINT size, UINT sizes, void* shaderByteCode, UINT shaderSizeByte)
{
	if (pBuffer)
		pBuffer->Release();
	if (pInputLayout)
		pInputLayout->Release();

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size * sizes;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = vertices;

	vertexSize = size;
	this->sizes = sizes;

	HRESULT res = pRenderSystem->pDevice->CreateBuffer(&bd, &data, &pBuffer);
	if (FAILED(res))
		throw std::exception("Create VertexBuffer failed!");

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		// SEMANTIC NAME, SEMANTIC INDEX, FORMAT, INPUT SLOT, ALIGNED BYTE OFFSET, INPUT SLOT CLASS, INSTANCE DATA STEP RATE
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 1,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT layoutSize = ARRAYSIZE(layout);

	res = pRenderSystem->pDevice->CreateInputLayout(layout, layoutSize, shaderByteCode, shaderSizeByte, &pInputLayout);
	if (FAILED(res))
		throw std::exception("VertexBuffer create input layout failed!");
}

void VertexBuffer::Release()
{
	if (pInputLayout)
		pInputLayout->Release();
	pBuffer->Release();
}
