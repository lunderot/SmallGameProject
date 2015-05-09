#include <fbxsdk.h>
#include <iostream>
#pragma comment (lib, "libfbxsdk.lib")

int main()
{
	FbxManager* sdkManager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(sdkManager, IOSROOT);

	sdkManager->SetIOSettings(ios);

	//Exporter
	FbxExporter* exporter = FbxExporter::Create(sdkManager, "");
	const char* fileName = "C://Users/Litet/Documents/GitHub/SmallGameProject/FBX Export/FBX Export/magnus.fbx";

	bool exportStatus = exporter->Initialize(fileName, -1, sdkManager->GetIOSettings());
	if (!exportStatus)
	{
		FBXSDK_printf("nu failade det.");
	}
	//Scene
	FbxScene* scene = FbxScene::Create(sdkManager, "magnusScene");
	exporter->Export(scene);
}