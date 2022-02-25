#include "ModelObject.h"
#include "Transform.h"
#include "ResourceStruct.h"
#include "PinelineStruct.h"
#include "GraphicsEngine.h"
#include "ConstantBuffer.h"

#include <exception>

ModelObject::ModelObject() { }

void ModelObject::Draw()
{
	if (!pMesh || pMaterials.empty())
		throw std::exception("No mesh or material, draw model object failed!");

	GraphicsEngine::GetInstance()->DrawMesh(pMesh, pMaterials);
}

void ModelObject::SetCBuffer(Constant c)
{
	cBuf = c;	
}

void ModelObject::UpdateByCBuf()
{
	for (MaterialPtr& mat : pMaterials)
	{
		mat->SetCBuffer(&cBuf, sizeof(Constant));
	}
}

void ModelObject::SetPosition(const Vector3 position)
{
	pTransform->SetPosition(position);
	cBuf.model = pTransform->GetMatrix();
	SetCBuffer(cBuf);	
}

void ModelObject::SetRotation(const Vector3 euler)
{
	pTransform->SetRotation(euler);
	cBuf.model = pTransform->GetMatrix();
	SetCBuffer(cBuf);
}

void ModelObject::SetScale(const Vector3 scale)
{
	pTransform->SetScale(scale);
	cBuf.model = pTransform->GetMatrix();
	SetCBuffer(cBuf);
}
