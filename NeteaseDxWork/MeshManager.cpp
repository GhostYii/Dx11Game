#include "MeshManager.h"
#include "Mesh.h"

MeshManager::MeshManager()
{
}

MeshPtr MeshManager::CreateMeshFromFile(const wchar_t* filePath)
{
    return std::static_pointer_cast<Mesh>(CreateResourceFromFile(filePath));
}

Resource* MeshManager::CreateResourceFromFileConcreate(const wchar_t* filePath)
{
    Mesh* ptr = nullptr;
    try
    {
        ptr = new Mesh(filePath);
    }
    catch (...) {}

    return ptr;
}
