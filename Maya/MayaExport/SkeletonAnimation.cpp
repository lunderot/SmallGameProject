#include "SkeletonAnimation.h"

using namespace std;

MStatus SkeletonAnimation::exportSkin(MObject& mayaObject, SkinAnimation& skin, map<const char*, int>& jointMap, map<const char*, unsigned int>& meshMap, vector<vector<int>> &influenceIndices, vector<vector<VertexInfluence>> &influenceWeights)
{
	MFnSkinCluster skinCluster(mayaObject);
	MDagPathArray influences;

	unsigned int numberOfInfluences = skinCluster.influenceObjects(influences);
	skin.numberOfInfluences = numberOfInfluences;
	
	unsigned int index;

	// It should only be possible to have one geometry per skin cluster, hence the 0
	index = skinCluster.indexForOutputConnection(0);
		
	MDagPath skinPath;
	skinCluster.getPathAtIndex(index, skinPath);

	MFnDependencyNode skinNode = skinPath.node();

	skin.skinMeshIndex = meshMap[skinNode.name().asChar()];

	vector<int> temp_ind;

	for (unsigned int j = 0; j < numberOfInfluences; j++)
	{
		MFnDependencyNode influenceNode = influences[j].node();

		temp_ind.push_back(jointMap[influenceNode.name().asChar()]);
	}
	influenceIndices.push_back(temp_ind);

	MItGeometry geomIter(skinPath);

	skin.skinVertexCount = geomIter.count();

	vector<VertexInfluence> temp_geom;

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
				tempWeights.push_back(make_pair(weights[x], temp_ind[x]));
			}

			while (tempWeights.size() < 4)
			{
				tempWeights.push_back(make_pair(0.0f, -1));
			}
		}
		else
		{
			vector<pair<double, int>> temp_pair;
			double totalWeight = 0.0f;

			for (unsigned int x = 0; x < weights.length(); x++)
			{
				temp_pair.push_back(make_pair(weights[x], temp_ind[x]));
			}

			sort(temp_pair.begin(), temp_pair.end());
			reverse(temp_pair.begin(), temp_pair.end());

			for (unsigned int x = 0; x < 4; x++)
			{
				tempWeights.push_back(temp_pair[x]);
				totalWeight += temp_pair[x].first;
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

		temp_geom.push_back(tempVert);

		geomIter.next();
	}

	influenceWeights.push_back(temp_geom);

	return MS::kSuccess;
}
