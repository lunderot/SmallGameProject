#ifndef COMPARE_H
#define COMPARE_H

#include "Defines.h"
#include "DataTypes.h"
#include "Scenes.h"
#include "Mesh.h"
#include <map>

class Compare
{
private:
	Scenes* lScenes;

	bool lStatus = false;
	bool isGolden = true, isTest = false;

	Mesh lMeshHandler;

	std::vector<MeshInfo> lGoldMeshInfo;
	std::vector<MeshInfo> lTestMeshInfo;
protected:
public:
	Compare(Scenes* pScenes);
	~Compare();

	void DoComparison(std::string pOutputFile);

	void GatherInfo(FbxNode* pGoldenRootNode, FbxNode* pTestRootNode);
	void TraverseScene(FbxNode* pNode, bool pType);

	void MeshCompare();

	std::string ReturnXYZW(int value);
};

#endif /* COMPARE_H */