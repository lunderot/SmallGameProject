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
		//FBXSDK_printf("current mesh node: %s\n", pNode->GetName());

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


		if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			for (int lVertexIndex = 0; lVertexIndex < pMesh->GetLayer(0)->GetNormals()->GetDirectArray().GetCount(); lVertexIndex++)
			{
				int lNormalIndex = 0;

				lNormalIndex = lVertexIndex;

				pMeshInfo.normals.push_back(lNormalElement->GetDirectArray().GetAt(lNormalIndex));
			}
		}

		if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
		{
			for (int lVertexIndex = 0; lVertexIndex < pMesh->GetLayer(0)->GetNormals()->GetIndexArray().GetCount(); lVertexIndex++)
			{
				int lNormalIndex = 0;

				lNormalIndex = lNormalElement->GetIndexArray().GetAt(lVertexIndex);

				pMeshInfo.normals.push_back(lNormalElement->GetDirectArray().GetAt(lNormalIndex));
			}
		}


			//FBXSDK_printf("normals for vertex[%d]: %f %f %f %f \n", lVertexIndex, pMeshInfo.normals[lVertexIndex][0], pMeshInfo.normals[lVertexIndex][1], pMeshInfo.normals[lVertexIndex][2], pMeshInfo.normals[lVertexIndex][3]);
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
		if (lUvElement->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			for (int lVertexIndex = 0; lVertexIndex < pMesh->GetLayer(0)->GetUVs()->GetDirectArray().GetCount(); lVertexIndex++)
			{
				int lUvIndex = 0;
				lUvIndex = lVertexIndex;
				pMeshInfo.uv.push_back(lUvElement->GetDirectArray().GetAt(lUvIndex));
			}
		}
		if (lUvElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
		{
			for (int lVertexIndex = 0; lVertexIndex < pMesh->GetLayer(0)->GetUVs()->GetIndexArray().GetCount(); lVertexIndex++)
			{
				int lUvIndex = 0;
				lUvIndex = lUvElement->GetIndexArray().GetAt(lVertexIndex);
				pMeshInfo.uv.push_back(lUvElement->GetDirectArray().GetAt(lUvIndex));
			}
		}


	}
}