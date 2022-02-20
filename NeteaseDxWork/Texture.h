#pragma once
#include "Resource.h"

#include <d3d11.h>
#include <string>
#include <wrl.h>

class Texture : public Resource
{
public:
	Texture(const wchar_t* fullPath);
	~Texture() = default;

private:
	Microsoft::WRL::ComPtr<ID3D11Resource> pTexture = nullptr;
};

