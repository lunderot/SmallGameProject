#include "Skeleton.h"

Skeleton::Skeleton()
{

}

Skeleton::~Skeleton()
{

}

void Skeleton::ExportSkeletons(FbxScene* scene, std::string fileName)
{
	Importer importer;
	if (!importer.importFile(fileName))
	{
		FBXSDK_printf("Failed to Import the file");
	}

	const ImporterJoint* importedJoints = importer.getJoint();
	const ImporterWeights* importedWeights = importer.getWeights();

	FbxNode* rootNode = scene->GetRootNode();

	//vector<>

	//for (unsigned int i = 0; i < importer.getNumJoints(); i++)
	//{
	//	
	//}

	for (unsigned int i = 0; i < importer.getNumSkins(); i++)
	{
		//FbxNode* CreateSkeleton = FbxNode::Create(scene, "");

		unsigned int counter;

		vector<int> jointIndecies;
		vector<ImporterJoint> joints;
		importedWeights[i].influenceIndices;

		for (unsigned int j = 0; j < importedWeights[i].numberOfInfluences; j++)
		{
			jointIndecies.push_back(importedWeights[i].influenceIndices[j]);
		}

		for (unsigned int j = 0; j < jointIndecies.size(); j++)
		{
			joints.push_back(importedJoints[jointIndecies[j]]);
		}

		while (counter < importedWeights[i].numberOfInfluences)
		{
			FbxString lNodeName(joints[counter].transform.name);

			FbxSkeleton* lSkeletonAttribute = FbxSkeleton::Create(scene, "");

			if (joints[counter].transform.parentID == -1)
			{
				lSkeletonAttribute->SetSkeletonType(FbxSkeleton::eRoot);
			}
			else
			{
				lSkeletonAttribute->SetSkeletonType(FbxSkeleton::eLimbNode);
			}

			FbxNode* lSkeleton = FbxNode::Create(scene, lNodeName.Buffer());

			lSkeleton->SetNodeAttribute(lSkeletonAttribute);


			counter++;
		}

		FbxString lRootName("ADolf");
		lRootName += "Root";

		FbxSkeleton* lSkeletonRootAttribute = FbxSkeleton::Create(scene, "");
		lSkeletonRootAttribute->SetSkeletonType(FbxSkeleton::eRoot);
		FbxNode* lSkeletonRoot = FbxNode::Create(scene, lRootName.Buffer());
		lSkeletonRoot->SetNodeAttribute(lSkeletonRootAttribute);
		lSkeletonRoot->LclTranslation.Set(FbxVector4());




	}
}