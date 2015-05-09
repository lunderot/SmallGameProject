#include "Compare.h"

Compare::Compare(Scenes* pScenes)
{
	this->lScenes = pScenes;
}

Compare::~Compare()
{

}

void Compare::DoComparison(std::string pOutputFile)
{
	FbxNode* lGoldenRootNode = this->lScenes->GetGolden()->GetRootNode();
	FbxNode* lTestRootNode = this->lScenes->GetTest()->GetRootNode();

	this->GatherInfo(lGoldenRootNode, lTestRootNode);

	FBXSDK_printf("Number of Gold meshes %d \n", this->lGoldMeshInfo.size());
	FBXSDK_printf("Number of Test meshes %d \n", this->lTestMeshInfo.size());
}

void Compare::GatherInfo(FbxNode* pGoldenRootNode, FbxNode* pTestRootNode)
{
	MeshInfo tempMeshInfo;

	int counter;

	for (counter = 0; counter <= pGoldenRootNode->GetChildCount(); counter++)
	{
		this->lStatus = this->lMeshHandler.GetInfo(pGoldenRootNode, tempMeshInfo);
		if (lStatus)
		{
			this->lGoldMeshInfo.push_back(tempMeshInfo);
			tempMeshInfo.normals.clear();
		}
		
		TraverseScene(pGoldenRootNode->GetChild(counter), this->isGolden);
	}

	for (counter = 0; counter <= pTestRootNode->GetChildCount(); counter++)
	{

		this->lStatus = this->lMeshHandler.GetInfo(pTestRootNode, tempMeshInfo);
		if (lStatus)
		{
			this->lTestMeshInfo.push_back(tempMeshInfo);
			tempMeshInfo.normals.clear();
		}

		TraverseScene(pTestRootNode->GetChild(counter), this->isTest);
	}
}

void Compare::TraverseScene(FbxNode* pNode, bool pType)
{
	int counter;
	MeshInfo tempMeshInfo;

	if (pNode)
	{	
		for (counter = 0; counter <= pNode->GetChildCount(); counter++)
		{	
			this->lStatus = this->lMeshHandler.GetInfo(pNode, tempMeshInfo);
			if (lStatus)
			{
				if (pType == this->isGolden)
				{
					this->lGoldMeshInfo.push_back(tempMeshInfo);
					tempMeshInfo.normals.clear();
				}
				else
				{
					this->lTestMeshInfo.push_back(tempMeshInfo);
					tempMeshInfo.normals.clear();
				}
			}

			TraverseScene(pNode->GetChild(counter), pType);
		}
	}
}