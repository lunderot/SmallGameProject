#ifndef COMPARE_H
#define COMPARE_H

#include "Defines.h"
#include "DataTypes.h"
#include "Scenes.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"
#include <map>

class Compare
{
private:
	Scenes* lScenes;

	bool lStatus = false;
	bool isGolden = true, isTest = false;

	Mesh lMeshHandler;
	Camera lCamHandler;
	Light lLightHandler;

	std::vector<MeshInfo> lGoldMeshInfo;
	std::vector<MeshInfo> lTestMeshInfo;
	std::vector<CamInfo> lGoldCamInfo;
	std::vector<CamInfo> lTestCamInfo;
	std::vector<LightInfo> lGoldLightInfo;
	std::vector<LightInfo> lTestLightInfo;
protected:
public:
	Compare(Scenes* pScenes);
	~Compare();

	void DoComparison(std::string pOutputFile);

	void GatherInfo(FbxNode* pGoldenRootNode, FbxNode* pTestRootNode);
	void TraverseScene(FbxNode* pNode, bool pType);

	void MeshCompare();
	void CameraCompare();
	void LightCompare();

	std::string ReturnXYZW(int value);
	std::string ReturnRGBA(int value);
};

#endif /* COMPARE_H */