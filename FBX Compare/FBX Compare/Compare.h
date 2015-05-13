#ifndef COMPARE_H
#define COMPARE_H

#include "Defines.h"
#include "DataTypes.h"
#include "Scenes.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Hierarchy.h"
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
	Material lMaterialHandler;
	Hierarchy lHierarchyHandler;

	std::vector<MeshInfo> lGoldMeshInfo;
	std::vector<MeshInfo> lTestMeshInfo;
	std::vector<CamInfo> lGoldCamInfo;
	std::vector<CamInfo> lTestCamInfo;
	std::vector<LightInfo> lGoldLightInfo;
	std::vector<LightInfo> lTestLightInfo;
	std::vector<std::pair<std::string, MaterialInfo>> lGoldMaterialInfo;
	std::vector<std::pair<std::string, MaterialInfo>> lTestMaterialInfo;
	std::vector<TransformNodeInfo> lGoldHierarchyNodes;
	std::vector<TransformNodeInfo> lTestHierarchyNodes;

	std::set<std::string> lGoldMaterialNames;
	std::set<std::string> lTestMaterialNames;
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
	void MaterialCompare();
	void HierarchyCompare();

	std::string ReturnXYZW(int value);
	std::string ReturnRGBA(int value);
};

#endif /* COMPARE_H */