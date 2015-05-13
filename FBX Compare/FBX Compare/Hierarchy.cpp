#include "Hierarchy.h"

Hierarchy::Hierarchy()
{

}

Hierarchy::~Hierarchy()
{

}

bool Hierarchy::GetInfo(FbxNode* pNode, TransformNodeInfo& pTransformNodeInfo)
{
	if (pNode)
	{
		FBXSDK_printf("current transform node: %s\n", pNode->GetName());

		this->GetTransforms(pNode, pTransformNodeInfo);
		this->GetChildren(pNode, pTransformNodeInfo);

		return true;
	}
	return false;
}

void Hierarchy::GetTransforms(FbxNode* pNode, TransformNodeInfo& pTransformNodeInfo)
{
	pTransformNodeInfo.rotation = pNode->LclRotation;
	pTransformNodeInfo.scale = pNode->LclScaling;
	pTransformNodeInfo.translation = pNode->LclTranslation;
}

void Hierarchy::GetChildren(FbxNode* pNode, TransformNodeInfo& pTransformNodeInfo)
{
	pTransformNodeInfo.node_name = pNode->GetName();
	
	std::string temp_name;
	for (int i = 0; i < pNode->GetChildCount(); i++)
	{
		temp_name = pNode->GetChildName(i);
		pTransformNodeInfo.child_names.push_back(temp_name);
	}
}