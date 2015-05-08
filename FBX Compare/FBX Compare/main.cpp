#include <string>
#include "Defines.h"
#include "Scenes.h"
#include "Compare.h"
#include "DataTypes.h"

#pragma comment(lib, "libfbxsdk.lib")

void SetSettings(FbxManager* pManager, FbxIOSettings* pSettings)
{
	pManager->SetIOSettings(pSettings);

	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_MATERIAL, true);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_TEXTURE, true);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_LINK, false);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_SHAPE, false);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_GOBO, false);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_ANIMATION, false);
	(*(pManager->GetIOSettings())).SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
}

int main(int argc, char** argv)
{
	FbxManager* lSdkManager = FbxManager::Create();
	FbxIOSettings* lSdkSettings = FbxIOSettings::Create(lSdkManager, IOSROOT);
	SetSettings(lSdkManager, lSdkSettings);

	Scenes scenes(lSdkManager, "file1.fbx", "file2.fbx");

	FBXSDK_printf("FBX Compare\n\n");

	Compare comparitor = Compare(&scenes);
	comparitor.DoComparison("out.txt");

	lSdkManager->Destroy();

	getchar();
}
