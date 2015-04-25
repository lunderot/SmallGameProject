#include "SkeletonAnimation.h"

using namespace std;

MStatus SkeletonAnimation::exportSkin(MObject& mayaObject, SkinAnimation& skin, map<const char*, int>& jointMap, map<const char*, unsigned int>& meshMap)
{
	MFnSkinCluster skinCluster(mayaObject);
	MDagPathArray influences;

	unsigned int numberOfInfluences = skinCluster.influenceObjects(influences);
	skin.numberOfInfluences = numberOfInfluences;

	skin.influenceIndices = new int[skin.numberOfInfluences];

	unsigned int index;

	// It should only be possible to have one geometry per skin cluster, hence the 0
	index = skinCluster.indexForOutputConnection(0);
		
	MDagPath skinPath;
	skinCluster.getPathAtIndex(index, skinPath);

	MFnDependencyNode skinNode = skinPath.node();

	skin.skinMeshIndex = meshMap[skinNode.name().asChar()];

	for (unsigned int j = 0; j < numberOfInfluences; j++)
	{
		MFnDependencyNode influenceNode = influences[j].node();

		skin.influenceIndices[j] = jointMap[influenceNode.name().asChar()];
	}

	MItGeometry geomIter(skinPath);

	skin.skinVertexCount = geomIter.count();

	skin.influenceWeights = new VertexInfluence[skin.skinVertexCount];

	while (!geomIter.isDone())
	{
		VertexInfluence tempVert;
		vector<pair<double, int>> tempWeights;

		MDoubleArray weights;
		unsigned int influenceCount;

		MObject comp = geomIter.component();

		skinCluster.getWeights(skinPath, comp, weights, influenceCount);

		if (numberOfInfluences <= 4)
		{
			for (unsigned int x = 0; x < weights.length(); x++)
			{
				tempWeights.push_back(make_pair(weights[x], skin.influenceIndices[x]));
			}

			while (tempWeights.size() < 4)
			{
				tempWeights.push_back(make_pair(0.0f, -1));
			}
		}
		else
		{
			vector<pair<double, int>> temp;
			double totalWeight = 0.0f;

			for (unsigned int x = 0; x < weights.length(); x++)
			{
				temp.push_back(make_pair(weights[x], skin.influenceIndices[x]));
			}

			sort(temp.begin(), temp.end());
			reverse(temp.begin(), temp.end());

			for (unsigned int x = 0; x < 4; x++)
			{
				tempWeights.push_back(temp[x]);
				totalWeight += temp[x].first;
			}

			double normalizer = 1.0f / totalWeight;

			for (unsigned int x = 0; x < 4; x++)
			{
				tempWeights[x].first = tempWeights[x].first * normalizer;
			}
		}

		for (unsigned int x = 0; x < 4; x++)
		{
			tempVert.influenceObject[x] = tempWeights[x].second;
			tempVert.weight[x] = tempWeights[x].first;
		}

		skin.influenceWeights[geomIter.index()] = tempVert;

		geomIter.next();
	}

	return MS::kSuccess;
}
