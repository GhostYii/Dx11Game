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
		
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = imageData.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = imageData.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;

		GraphicsEngine::GetInstance()->GetRenderSystem()->pDevice->CreateShaderResourceView(pTexture.Get(), &desc, pShaderResourceView.GetAddressOf());

		if (FAILED(res))
			throw std::exception("Create Texture failed!");
	}
	else
	{
		throw std::exception("Texture resource load failed!");
	}
}
