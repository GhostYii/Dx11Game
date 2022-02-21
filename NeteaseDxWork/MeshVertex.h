#pragma once
#include "Vector2.h"
#include "Vector3.h"

class MeshVertex
{
public:
	MeshVertex() : position(), texcoord() {}
	MeshVertex(Vector3 position, Vector2 texcoord) : position(position), texcoord(texcoord) {}
	MeshVertex(const MeshVertex& other) : position(other.position), texcoord(other.texcoord) {}

	~MeshVertex() = default;

private:
	Vector3 position;
	Vector2 texcoord;
};