#include "Mesh.h"
Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

bool Mesh::GetInfo(FbxNode* pNode, MeshInfo& pMeshInfo)
{
	FbxMesh* lMesh = pNode->GetMesh();
	
	if (lMesh)
	{
		FBXSDK_printf("current mesh node: %s\n", pNode->GetName());

		this->GetNormals(lMesh, pMeshInfo);
		this->GetPosition(lMesh, pMeshInfo);
		this->GetUV(lMesh, pMeshInfo);

		return true;
	}
	return false;
}

void Mesh::GetNormals(FbxMesh* pMesh, MeshInfo& pMeshInfo)
{
	FbxGeometryElementNormal* lNormalElement = pMesh->GetElementNormal();

	if (lNormalElement)
	{
		for (int lVertexIndex = 0; lVertexIndex < pMesh->GetControlPointsCount(); lVertexIndex++)
		{
			int lNormalIndex = 0;

			if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				lNormalIndex = lVertexIndex;
			}

			if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				lNormalIndex = lNormalElement->GetIndexArray().GetAt(lVertexIndex);
			}

			pMeshInfo.normals.push_back(lNormalElement->GetDirectArray().GetAt(lNormalIndex));

			//FBXSDK_printf("normals for vertex[%d]: %f %f %f %f \n", lVertexIndex, pMeshInfo.normals[lVertexIndex][0], pMeshInfo.normals[lVertexIndex][1], pMeshInfo.normals[lVertexIndex][2], pMeshInfo.normals[lVertexIndex][3]);
		}
	}
}

void Mesh::GetPosition(FbxMesh* pMesh, MeshInfo& pMeshInfo)
{
	for (int lVertexIndex = 0; lVertexIndex < pMesh->GetControlPointsCount(); lVertexIndex++)
	{
		pMeshInfo.position.push_back(pMesh->GetControlPointAt(lVertexIndex));

		//FBXSDK_printf("position for vertex[%d]: %f %f %f %f \n", lVertexIndex, pMeshInfo.position[lVertexIndex][0], pMeshInfo.position[lVertexIndex][1], pMeshInfo.position[lVertexIndex][2], pMeshInfo.position[lVertexIndex][3]);
	}
}

void Mesh::GetUV(FbxMesh* pMesh, MeshInfo& pMeshInfo)
{
	FbxGeometryElementUV* lUvElement = pMesh->GetElementUV();

	if (lUvElement)
	{
		for (int lVertexIndex = 0; lVertexIndex < pMesh->GetControlPointsCount(); lVertexIndex++)
		{
			int lUvIndex = 0;

			if (lUvElement->GetReferenceMode() == FbxGeometryElement::eDirect)
			{
				lUvIndex = lVertexIndex;
			}
			if (lUvElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				lUvIndex = lUvElement->GetIndexArray().GetAt(lVertexIndex);
			}

			pMeshInfo.uv.push_back(lUvElement->GetDirectArray().GetAt(lUvIndex));
		}
	}
}