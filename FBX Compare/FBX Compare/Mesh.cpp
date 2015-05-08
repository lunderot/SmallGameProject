#include "Mesh.h"
Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::GetInfo(FbxNode* pNode, MeshInfo& pMeshInfo)
{
	FbxMesh* lMesh = pNode->GetMesh();
	
	if (lMesh)
	{
		FBXSDK_printf("current mesh node: %s\n", pNode->GetName());

		this->GetNormals(lMesh, pMeshInfo);
	}
}

void Mesh::GetNormals(FbxMesh* pMesh, MeshInfo& pMeshInfo)
{
	FbxGeometryElementNormal* lNormalElement = pMesh->GetElementNormal();

	if (lNormalElement)
	{
		if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			for (unsigned int lVertexIndex = 0; lVertexIndex < pMesh->GetControlPointsCount(); lVertexIndex++)
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
}