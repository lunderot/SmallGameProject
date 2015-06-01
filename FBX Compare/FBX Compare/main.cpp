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

	const char* lFileName1 = nullptr;
	const char* lFileName2 = nullptr;

	if (argc < 3)
	{
		FBXSDK_printf("No input files specified!\n\n");
	}
	else
	{
		lFileName1 = argv[1];
		lFileName2 = argv[2];
	}

	if (lFileName1 && lFileName2)
	{
		Scenes scenes(lSdkManager, std::string(lFileName1), std::string(lFileName2));

		FBXSDK_printf("FBX Compare\n\n");
		FBXSDK_printf("Differences between files:\n\n");

		Compare comparitor = Compare(&scenes);
		comparitor.DoComparison("out.txt");
	}
	else
	{
		FBXSDK_printf("No input files specified!\n\n");
	}

	lSdkManager->Destroy();

	getchar();
}
