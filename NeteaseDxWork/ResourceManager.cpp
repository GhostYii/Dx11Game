#include "ResourceManager.h"
#include <filesystem>

ResourcePtr ResourceManager::CreateResourceFromFile(const wchar_t* filePath)
{
	std::wstring fullPath = std::filesystem::absolute(filePath);

	auto iter = resourceMap.find(fullPath);
	if (iter != resourceMap.end())
		return iter->second;

	Resource* resource = this->CreateResourceFromFileConcreate(fullPath.c_str());

	if (resource)
	{
		ResourcePtr ptr(resource);
		resourceMap[fullPath] = ptr;
		return ptr;
	}

	return nullptr;
}
