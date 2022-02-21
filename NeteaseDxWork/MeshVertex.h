#pragma once
#include "Vector2.h"
#include "Vector3.h"

class MeshVertex
{
public:
	MeshVertex() : position(), texcoord(), normal() {}
	MeshVertex(Vector3 position, Vector2 texcoord, Vector3 normal) : position(position), texcoord(texcoord), normal(normal) {}
	MeshVertex(const MeshVertex& other) : position(other.position), texcoord(other.texcoord), normal(other.normal) {}

	~MeshVertex() = default;

private:
	Vector3 position;
	Vector2 texcoord;
	Vector3 normal;
};