#include "DeviceContext.h"
#include "PinelineStruct.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* pDeviceContext, RenderSystem* rs) 
	: pDeviceContext(pDeviceContext), pRenderSystem(rs)
{
}

DeviceContext::~DeviceContext()
{
	pDeviceContext->Release();
}

void DeviceContext::ClearRenderTargetColor(SwapChain* swapChain, float r, float g, float b, float a)
{
	const float color[] = { r,g,b,a };
	pDeviceContext->ClearRenderTargetView(swapChain->pTargetView, color);
	pDeviceContext->OMSetRenderTargets(1, &swapChain->pTargetView, NULL);
}

void DeviceContext::SetVertexBuffer(VertexBuffer* pBuffer)
{
	UINT stride = pBuffer->vertexSize;
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pBuffer->pBuffer, &stride, &offset);

	pDeviceContext->IASetInputLayout(pBuffer->pInputLayout);
}

void DeviceContext::SetIndexBuffer(IndexBuffer* pBuffer)
{
	pDeviceContext->IASetIndexBuffer(pBuffer->pBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::DrawTriangleList(UINT vertexSize, UINT startIndex)
{
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->Draw(vertexSize, startIndex);
}

void DeviceContext::DrawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation)
{
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->DrawIndexed(indexCount, startIndexLocation, startVertexIndex);
}

void DeviceContext::SetViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	pDeviceContext->RSSetViewports(1, &vp);
}

void DeviceContext::SetVertexShader(VertexShader* shader)
{
	pDeviceContext->VSSetShader(shader->pVertexShader, nullptr, 0);
}

void DeviceContext::SetPixelShader(PixelShader* shader)
{
	pDeviceContext->PSSetShader(shader->pPixelShader, nullptr, 0);
}

void DeviceContext::VSSetConstantBuffer(/*VertexShader* shader,*/ ConstantBuffer* cBuffer)
{
	pDeviceContext->VSSetConstantBuffers(0, 1, &cBuffer->pBuffer);
}

void DeviceContext::PSSetConstantBuffer(/*PixelShader* shader,*/ ConstantBuffer* cBuffer)
{
	pDeviceContext->PSSetConstantBuffers(0, 1, &cBuffer->pBuffer);
}
