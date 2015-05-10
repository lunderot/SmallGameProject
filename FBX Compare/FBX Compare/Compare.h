#ifndef COMPARE_H
#define COMPARE_H

#include "Defines.h"
#include "DataTypes.h"
#include "Scenes.h"
#include "Mesh.h"
#include "Camera.h"
#include <map>

class Compare
{
private:
	Scenes* lScenes;

	bool lStatus = false;
	bool isGolden = true, isTest = false;

	Mesh lMeshHandler;
	Camera lCamHandler;

	std::vector<MeshInfo> lGoldMeshInfo;
	std::vector<MeshInfo> lTestMeshInfo;
	std::vector<CamInfo> lGoldCamInfo;
	std::vector<CamInfo> lTestCamInfo;
protected:
public:
	Compare(Scenes* pScenes);
	~Compare();

	void DoComparison(std::string pOutputFile);

	void GatherInfo(FbxNode* pGoldenRootNode, FbxNode* pTestRootNode);
	void TraverseScene(FbxNode* pNode, bool pType);

	void MeshCompare();
	void CameraCompare();

	std::string ReturnXYZW(int value);
};

#endif /* COMPARE_H */