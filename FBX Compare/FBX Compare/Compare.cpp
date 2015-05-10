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

	this->MeshCompare();
}

void Compare::MeshCompare()
{
	if (this->lGoldMeshInfo.size() != this->lTestMeshInfo.size())
	{
		FBXSDK_printf("\nThe number of meshes differ between files\n");
		FBXSDK_printf("Early error out!\n");
	}
	else
	{
		for (unsigned int i = 0; i < this->lGoldMeshInfo.size(); i++)
		{
			for (unsigned int j = 0; j < this->lGoldMeshInfo[i].normals.size(); j++)
			{
				if (this->lGoldMeshInfo[i].normals.size() != this->lTestMeshInfo[i].normals.size())
				{
					FBXSDK_printf("\nThe number of normals differ in mesh number %d\n", i);
					FBXSDK_printf("Early error out!\n");
				}
				else
				{
					for (unsigned int k = 0; k < 4; k++)
					{
						if ((abs(this->lGoldMeshInfo[i].normals[j][k]) - abs(this->lTestMeshInfo[i].normals[j][k])) > EPSILON || (abs(this->lGoldMeshInfo[i].normals[j][k]) - abs(this->lTestMeshInfo[i].normals[j][k])) < -EPSILON)
						{
							FBXSDK_printf("Mesh [%d], normal [%d], coord %s differ by more than an epsilon: % f\n", i, j, this->ReturnXYZW(k), (abs(this->lGoldMeshInfo[i].normals[j][k]) - abs(this->lTestMeshInfo[i].normals[j][k])));
						}
					}
				}
			}

			for (unsigned int j = 0; j < this->lGoldMeshInfo[i].position.size(); j++)
			{
				if (this->lGoldMeshInfo[i].position.size() != this->lTestMeshInfo[i].position.size())
				{
					FBXSDK_printf("\nThe number of vertex positions differ in mesh number %d\n", i);
					FBXSDK_printf("Early error out!\n");
				}
				else
				{
					for (unsigned int k = 0; k < 4; k++)
					{
						if ((abs(this->lGoldMeshInfo[i].position[j][k]) - abs(this->lTestMeshInfo[i].position[j][k])) > EPSILON || (abs(this->lGoldMeshInfo[i].position[j][k]) - abs(this->lTestMeshInfo[i].position[j][k])) < -EPSILON)
						{
							FBXSDK_printf("Mesh [%d], vert pos [%d], coord %s differ by more than an epsilon: % f\n", i, j, this->ReturnXYZW(k), (abs(this->lGoldMeshInfo[i].position[j][k]) - abs(this->lTestMeshInfo[i].position[j][k])));
						}
					}
				}
			}

			for (unsigned int j = 0; j < this->lGoldMeshInfo[i].uv.size(); j++)
			{
				if (this->lGoldMeshInfo[i].uv.size() != this->lTestMeshInfo[i].uv.size())
				{
					FBXSDK_printf("\nThe number of UV coordinates differ in mesh number %d\n", i);
					FBXSDK_printf("Early error out!\n");
				}
				else
				{
					for (unsigned int k = 0; k < 2; k++)
					{
						if ((abs(this->lGoldMeshInfo[i].uv[j][k]) - abs(this->lTestMeshInfo[i].uv[j][k])) > EPSILON || (abs(this->lGoldMeshInfo[i].uv[j][k]) - abs(this->lTestMeshInfo[i].uv[j][k])) < -EPSILON)
						{
							FBXSDK_printf("Mesh [%d], UV [%d], coord %s differ by more than an epsilon: % f\n", i, j, this->ReturnXYZW(k), (abs(this->lGoldMeshInfo[i].uv[j][k]) - abs(this->lTestMeshInfo[i].uv[j][k])));
						}
					}
				}
			}
		}
	}
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
			tempMeshInfo.position.clear();
			tempMeshInfo.uv.clear();
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
			tempMeshInfo.position.clear();
			tempMeshInfo.uv.clear();
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
					tempMeshInfo.position.clear();
					tempMeshInfo.uv.clear();
				}
				else
				{
					this->lTestMeshInfo.push_back(tempMeshInfo);
					tempMeshInfo.normals.clear();
					tempMeshInfo.position.clear();
					tempMeshInfo.uv.clear();
				}
			}

			TraverseScene(pNode->GetChild(counter), pType);
		}
	}
}

std::string Compare::ReturnXYZW(int value)
{
	if (value == 0){ return "x"; }
	else if(value == 1){ return "y"; }
	else if(value == 2){ return "z"; }
	else if(value == 3){ return "w"; }
	else { return "unk"; }
}