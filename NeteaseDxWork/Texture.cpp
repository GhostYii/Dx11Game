#include "Texture.h"
#include "GraphicsEngine.h"

#include <DirectXTex.h>

Texture::Texture(const wchar_t* fullPath) : Resource(fullPath)
{
	DirectX::ScratchImage imageData;
	HRESULT res = DirectX::LoadFromWICFile(fullPath, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, imageData);

	if (SUCCEEDED(res))
	{
		res = DirectX::CreateTexture(GraphicsEngine::GetInstance()->GetRenderSystem()->pDevice, imageData.GetImages(), imageData.GetImageCount(), imageData.GetMetadata(), pTexture.GetAddressOf());
		
		if (FAILED(res))
			throw std::exception("CreateTexture failed!");

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = imageData.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = (UINT)imageData.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;

		D3D11_SAMPLER_DESC samplerSesc = {};
		samplerSesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerSesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerSesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerSesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerSesc.MinLOD = 0;
		samplerSesc.MaxLOD = (FLOAT)imageData.GetMetadata().mipLevels;

		res = GraphicsEngine::GetInstance()->GetRenderSystem()->pDevice->CreateSamplerState(&samplerSesc, pSamplerState.GetAddressOf());
		
		if (FAILED(res))
			throw std::exception("Create Sampler State failed!");

		GraphicsEngine::GetInstance()->GetRenderSystem()->pDevice->CreateShaderResourceView(pTexture.Get(), &desc, pShaderResourceView.GetAddressOf());

		if (FAILED(res))
			throw std::exception("Create Shader ResourceView failed!");
	}
	else
	{
		throw std::exception("Texture resource load failed!");
	}
}
