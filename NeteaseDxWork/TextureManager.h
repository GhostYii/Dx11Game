#pragma once
#include "ResourceManager.h"

class TextureManager : public ResourceManager
{
public:
	TextureManager();
	virtual ~TextureManager() = default;

public:
	TexturePtr CreateTextureFromFile(const wchar_t* filePath);
protected:
	// Inherited via ResourceManager
	virtual Resource* CreateResourceFromFileConcreate(const wchar_t* filePath) override;
};

