#pragma once
#include <unordered_map>
#include <string>

#include "ResourceClasses.h"
#include "Resource.h"

class ResourceManager
{
public:
	ResourceManager() = default;
	virtual ~ResourceManager() = default;

public:
	ResourcePtr CreateResourceFromFile(const wchar_t* filePath);

protected:
	virtual Resource* CreateResourceFromFileConcreate(const wchar_t* filePath) = 0;

private:
	std::unordered_map<std::wstring, ResourcePtr> resourceMap;
};

