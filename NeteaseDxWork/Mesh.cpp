#include "Mesh.h"
#include "GraphicsEngine.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "MeshVertex.h"

#include "Utility.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

//#include <locale>


Mesh::Mesh(const wchar_t* fullPath) : Resource(fullPath)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string error;
	//std::string filename = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(fullPath);	
	std::string filename = convert::wstring2string(fullPath);

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &error, filename.c_str());

	if (!res)
	{
		throw std::exception("Mesh load failed!");
	}

	if (shapes.size() > 1)
	{
		throw std::exception("Mesh shapes error!");
	}

	std::vector<MeshVertex> meshVerts;
	std::vector<unsigned int> meshIndices;

	for (size_t i = 0; i < shapes.size(); i++)
	{
		size_t indexOffset = 0;
		meshVerts.reserve(shapes[i].mesh.indices.size());
		meshIndices.reserve(shapes[i].mesh.indices.size());

		for (size_t j = 0; j < shapes[i].mesh.num_face_vertices.size(); j++)
		{
			unsigned char faceVerCount = shapes[i].mesh.num_face_vertices[j];
			for (unsigned char k = 0; k < faceVerCount; k++)
			{
				tinyobj::index_t index = shapes[i].mesh.indices[indexOffset + k];
				tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
				tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
				tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

				tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
				tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

				MeshVertex vert(Vector3(vx, vy, vz), Vector2(tx, ty));
				meshVerts.push_back(vert);

				meshIndices.push_back(indexOffset + k);
			}

			indexOffset += faceVerCount;

		}
	}

	void* shaderByteCode = nullptr;
	size_t shaderSize = 0;

	GraphicsEngine::GetInstance()->GetDefaultVertexShaderByteCodeAndSize(&shaderByteCode, &shaderSize);
	pVertexBuffer = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateVertexBuffer(&meshVerts[0], (UINT)sizeof(MeshVertex), (UINT)meshVerts.size(), shaderByteCode, (UINT)shaderSize);

	pIndexBuffer = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateIndexBuffer(&meshIndices[0], (UINT)meshIndices.size());
	
}
