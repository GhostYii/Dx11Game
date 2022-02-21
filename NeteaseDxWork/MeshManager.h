#pragma once
#include "ResourceManager.h"
#include "ResourceClasses.h"

class MeshManager : public ResourceManager
{
public:
	MeshManager();
	virtual ~MeshManager() = default;

public:
	MeshPtr CreateMeshFromFile(const wchar_t* filePath);

protected:
	// Inherited via ResourceManager
	virtual Resource* CreateResourceFromFileConcreate(const wchar_t* filePath) override;
};

