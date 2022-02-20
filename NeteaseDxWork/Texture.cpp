#include "Texture.h"
#include "GraphicsEngine.h"

#include <DirectXTex.h>

Texture::Texture(const wchar_t* fullPath) : Resource(fullPath)
{
	DirectX::ScratchImage imageData;
	HRESULT res = DirectX::LoadFromWICFile(fullPath, DirectX::WIC_FLAGS_NONE, nullptr, imageData);

	if (SUCCEEDED(res))
	{
		res = DirectX::CreateTexture(GraphicsEngine::GetInstance()->GetRenderSystem()->pDevice, imageData.GetImages(), imageData.GetImageCount(), imageData.GetMetadata(), pTexture.GetAddressOf());
		
		if (FAILED(res))
			throw std::exception("Create Texture failed!");
	}
	else
	{
		throw std::exception("Texture resource load failed!");
	}
}
