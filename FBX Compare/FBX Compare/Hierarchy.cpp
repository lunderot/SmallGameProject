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
		//FBXSDK_printf("current transform node: %s\n", pNode->GetName());

		this->GetTransforms(pNode, pTransformNodeInfo);
		this->GetChildren(pNode, pTransformNodeInfo);

		return true;
	}
	return false;
}

void Hierarchy::GetTransforms(FbxNode* pNode, TransformNodeInfo& pTransformNodeInfo)
{
	pTransformNodeInfo.rotation = pNode->LclRotation.Get();
	pTransformNodeInfo.scale = pNode->LclScaling.Get();
	pTransformNodeInfo.translation = pNode->LclTranslation.Get();
}

void Hierarchy::GetChildren(FbxNode* pNode, TransformNodeInfo& pTransformNodeInfo)
{
	pTransformNodeInfo.node_name = pNode->GetName();
	
	std::string temp_name;
	for (int i = 0; i < pNode->GetChildCount(); i++)
	{
		//temp_name = pNode->GetChildName(i);
		FbxNode* temp_node = pNode->GetChild(i);
		temp_name = temp_node->GetName();
		
		pTransformNodeInfo.child_names.push_back(temp_name);
	}
}