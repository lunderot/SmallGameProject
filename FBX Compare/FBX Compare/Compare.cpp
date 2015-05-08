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

	MeshInfo tempMeshInfo;

	this->meshHandler.GetInfo(lGoldenRootNode, tempMeshInfo);
}