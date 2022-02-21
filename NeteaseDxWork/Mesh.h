#pragma once
#include "Resource.h"
#include "GraphicsClasses.h"

class Mesh : public Resource
{
	friend class DeviceContext;
public:
	Mesh(const wchar_t* fullPath);
	~Mesh() = default;

public:
	const VertexBufferPtr& GetVertexBuffer() { return pVertexBuffer; }
	const IndexBufferPtr& GetIndexBuffer() { return pIndexBuffer; }

private:
	VertexBufferPtr pVertexBuffer = nullptr;
	IndexBufferPtr pIndexBuffer = nullptr;
};

