#pragma once
#include <vector>
#include "Mesh.h"
#include "types.h"
#include "Object.h"
#include "GraphicsClasses.h"
#include "ComponentClass.h"
#include "ResourceClasses.h"

class ModelObject : public Object
{
public:
	ModelObject();
	virtual ~ModelObject() = default;

public:
	// 绘制模型对象
	void Draw();

	void SetCBuffer(Constant c);
	void UpdateByCBuf();

	void SetPosition(const Vector3 position);
	void SetRotation(const Vector3 euler);
	void SetScale(const Vector3 scale);

public:
	virtual void Start() {}
	virtual void Update(float frameTime) {}

public:
	MeshPtr pMesh = nullptr;
	std::vector<MaterialPtr> pMaterials;
	ConstantBufferPtr pConstantBuffer = nullptr;

private:
	Constant cBuf;
};