#include "DeviceContext.h"
#include "PinelineStruct.h"

#include "Texture.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* pDeviceContext, RenderSystem* rs) 
	: pDeviceContext(pDeviceContext), pRenderSystem(rs)
{
}

DeviceContext::~DeviceContext()
{
	pDeviceContext->Release();
}

void DeviceContext::ClearRenderTargetColor(SwapChainPtr pSwapChain, float r, float g, float b, float a)
{
	const float color[] = { r,g,b,a };
	pDeviceContext->ClearRenderTargetView(pSwapChain->pRenderTargetView.Get(), color);
	pDeviceContext->ClearDepthStencilView(pSwapChain->pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	pDeviceContext->OMSetRenderTargets(1, pSwapChain->pRenderTargetView.GetAddressOf(), pSwapChain->pDepthStencilView.Get());
}

void DeviceContext::SetVertexBuffer(const VertexBufferPtr& pBuffer)
{
	UINT stride = pBuffer->vertexSize;
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, pBuffer->pBuffer.GetAddressOf(), &stride, &offset);
	//pDeviceContext->IASetVertexBuffers(0, 1, &pBuffer->pBuffer, &stride, &offset);

	pDeviceContext->IASetInputLayout(pBuffer->pInputLayout.Get());
}

void DeviceContext::SetIndexBuffer(const IndexBufferPtr& pBuffer)
{
	pDeviceContext->IASetIndexBuffer(pBuffer->pBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
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

void DeviceContext::SetVertexShader(const VertexShaderPtr& shader)
{
	pDeviceContext->VSSetShader(shader->pVertexShader.Get(), nullptr, 0);
}

void DeviceContext::SetPixelShader(const PixelShaderPtr& shader)
{
	pDeviceContext->PSSetShader(shader->pPixelShader.Get(), nullptr, 0);
}

void DeviceContext::VSSetConstantBuffer(const ConstantBufferPtr& cBuffer)
{
	pDeviceContext->VSSetConstantBuffers(0, 1, cBuffer->pBuffer.GetAddressOf());
}

void DeviceContext::PSSetConstantBuffer(const ConstantBufferPtr& cBuffer)
{
	pDeviceContext->PSSetConstantBuffers(0, 1, cBuffer->pBuffer.GetAddressOf());
}

void DeviceContext::VSSetTexture(const TexturePtr* textures, UINT size)
{
	ID3D11ShaderResourceView* resList[32u];
	ID3D11SamplerState* samplerList[32u];

	for (size_t i = 0; i < size; i++)
	{
		resList[i] = textures[i]->pShaderResourceView.Get();
		samplerList[i] = textures[i]->pSamplerState.Get();
	}

	pDeviceContext->VSSetShaderResources(0, size, resList);
	pDeviceContext->VSSetSamplers(0, size, samplerList);
}

void DeviceContext::PSSetTexture(const TexturePtr* textures, UINT size)
{
	ID3D11ShaderResourceView* resList[32u];
	ID3D11SamplerState* samplerList[32u];

	for (size_t i = 0; i < size; i++)
	{
		resList[i] = textures[i]->pShaderResourceView.Get();
		samplerList[i] = textures[i]->pSamplerState.Get();
	}
	pDeviceContext->PSSetShaderResources(0, size, resList);
	pDeviceContext->PSSetSamplers(0, size, samplerList);
}

void DeviceContext::PSSetTexture(const TexturePtr& texture)
{
	pDeviceContext->PSSetShaderResources(0, 1, texture->pShaderResourceView.GetAddressOf());
	pDeviceContext->PSSetSamplers(0, 1, texture->pSamplerState.GetAddressOf());
}
