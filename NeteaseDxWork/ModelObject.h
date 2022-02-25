#pragma once
#include <vector>
#include "Mesh.h"
#include "types.h"
#include "GraphicsClasses.h"
#include "ComponentClass.h"
#include "ResourceClasses.h"

class ModelObject
{
public:
	ModelObject(TransformPtr pTransform);
	~ModelObject() = default;

public:
	// 绘制模型对象
	void Draw();

	void SetCBuffer(Constant c);
	void UpdateByCBuf();

	void SetPosition(const Vector3 position);
	void SetRotation(const Vector3 euler);
	void SetScale(const Vector3 scale);

public:
	TransformPtr pTransform = nullptr;
	MeshPtr pMesh = nullptr;
	std::vector<MaterialPtr> pMaterials;
	ConstantBufferPtr pConstantBuffer = nullptr;

private:
	Constant cBuf;
};