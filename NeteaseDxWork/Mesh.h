#pragma once
#include "Resource.h"
#include "GraphicsClasses.h"

#include <vector>

struct MaterialSlot
{
	size_t startIndex = 0;
	size_t numIndices = 0;
	size_t materialId = 0;
};

class Mesh : public Resource
{
	friend class DeviceContext;
public:
	Mesh(const wchar_t* fullPath);
	~Mesh() = default;

public:
	const VertexBufferPtr& GetVertexBuffer() { return pVertexBuffer; }
	const IndexBufferPtr& GetIndexBuffer() { return pIndexBuffer; }

	const MaterialSlot& GetMaterialSlot(unsigned int index);
	size_t GetMaterialSlotCount();

private:
	VertexBufferPtr pVertexBuffer = nullptr;
	IndexBufferPtr pIndexBuffer = nullptr;

	std::vector<MaterialSlot> materialSlots;
};

