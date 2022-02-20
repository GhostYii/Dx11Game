#include "TextureManager.h"
#include "Texture.h"

TextureManager::TextureManager() : ResourceManager()
{
}

TexturePtr TextureManager::CreateTextureFromFile(const wchar_t* filePath)
{
	return std::static_pointer_cast<Texture>(CreateResourceFromFile(filePath));
}

Resource* TextureManager::CreateResourceFromFileConcreate(const wchar_t* filePath)
{
	Texture* ptr = nullptr;
	try
	{
		ptr = new Texture(filePath);
	}
	catch(...) {}

	return ptr;
}
