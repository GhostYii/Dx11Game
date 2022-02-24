#include "Mesh.h"
#include "GraphicsEngine.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "MeshVertex.h"

#include "Utility.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Mesh::Mesh(const wchar_t* fullPath) : Resource(fullPath)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string error, warning;
	std::string filename = convert::wstring2string(fullPath);

	std::string mtlDir = filename.substr(0, filename.find_last_of("\\/"));
		
	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warning, &error, filename.c_str(), mtlDir.c_str());

	if (!error.empty())
	{
		throw std::exception("Obj load error!");
	}

	if (!res)
	{
		throw std::exception("Mesh load failed!");
	}

	std::vector<MeshVertex> meshVerts;
	std::vector<unsigned int> meshIndices;

	size_t vertexIndexSize = 0;

	for (size_t i = 0; i < shapes.size(); i++)
	{
		vertexIndexSize += shapes[i].mesh.indices.size();
	}

	meshVerts.reserve(vertexIndexSize);
	meshIndices.reserve(vertexIndexSize);

	if (materials.size() == 0)
		materialSlots.push_back(MaterialSlot{ 0,vertexIndexSize,0 });

	materialSlots.resize(materials.size());	

	size_t indexOffset = 0;

	for (size_t m = 0; m < materials.size(); m++)
	{
		materialSlots[m].startIndex = indexOffset;
		materialSlots[m].materialId = m;

		for (size_t i = 0; i < shapes.size(); i++)
		{
			size_t offset = 0;
			for (size_t j = 0; j < shapes[i].mesh.num_face_vertices.size(); j++)
			{
				if (shapes[i].mesh.material_ids[j] != m)
					continue;

				unsigned char faceVerCount = shapes[i].mesh.num_face_vertices[j];
				for (unsigned char k = 0; k < faceVerCount; k++)
				{
					tinyobj::index_t index = shapes[i].mesh.indices[indexOffset + k];
					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
					tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

					tinyobj::real_t nx = attribs.normals[index.normal_index * 3 + 0];
					tinyobj::real_t ny = attribs.normals[index.normal_index * 3 + 1];
					tinyobj::real_t nz = attribs.normals[index.normal_index * 3 + 2];

					MeshVertex vert(Vector3(vx, vy, vz), Vector2(tx, ty), Vector3(nx, ny, nz));
					meshVerts.push_back(vert);

					meshIndices.push_back(indexOffset + k);
				}
				offset += faceVerCount;
				indexOffset += faceVerCount;

			}
		}

		materialSlots[m].numIndices = indexOffset - materialSlots[m].startIndex;
	}

	void* shaderByteCode = nullptr;
	size_t shaderSize = 0;

	GraphicsEngine::GetInstance()->GetDefaultVertexShaderByteCodeAndSize(&shaderByteCode, &shaderSize);
	pVertexBuffer = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateVertexBuffer(&meshVerts[0], (UINT)sizeof(MeshVertex), (UINT)meshVerts.size(), shaderByteCode, (UINT)shaderSize);

	pIndexBuffer = GraphicsEngine::GetInstance()->GetRenderSystem()->CreateIndexBuffer(&meshIndices[0], (UINT)meshIndices.size());
	
}

const MaterialSlot& Mesh::GetMaterialSlot(unsigned int index)
{
	if (index >= materialSlots.size())
		return MaterialSlot();

	return materialSlots[index];
}

size_t Mesh::GetMaterialSlotCount()
{
	return materialSlots.size();
}
