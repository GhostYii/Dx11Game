#pragma once
#include <string>

class Resource
{
public:
	Resource(const wchar_t* fullPath);
	~Resource() = default;

protected:
	std::wstring fullPath;
};

