#include "Includes.h"
#include "Importer.h"

int main()
{

	FbxManager* sdkManager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(sdkManager, IOSROOT);
	ios->SetBoolProp(EXP_FBX_MATERIAL,				true);
	ios->SetBoolProp(EXP_FBX_TEXTURE,				true);
	ios->SetBoolProp(EXP_FBX_SHAPE,					true);
	ios->SetBoolProp(EXP_FBX_GOBO,					true);
	ios->SetBoolProp(EXP_FBX_ANIMATION,				true);
	ios->SetBoolProp(EXP_FBX_GLOBAL_SETTINGS,		true);

	sdkManager->SetIOSettings(ios);

	//Exporter
	FbxExporter* exporter = FbxExporter::Create(sdkManager, "");
	const char* fileName = "C://Users/Litet/Documents/GitHub/SmallGameProject/FBX Export/FBX Export/testFile.fbx";

	bool exportStatus = exporter->Initialize(fileName, 0, sdkManager->GetIOSettings());
	if (!exportStatus)
	{
		FBXSDK_printf("Failed to initialize the exporter!");
	}
	//Scene
	FbxScene* scene = FbxScene::Create(sdkManager, "aScene");

	// Mesh
	Mesh mesh;

	// Fill the FBX scene with data
	mesh.GetMesh(scene, "C://Users/Litet/Documents/GitHub/SmallGameProject/FBX Export/FBX Export/BackgroundPlane.bin");

	exporter->Export(scene);
	exporter->Destroy();
	sdkManager->Destroy();
	std::getchar();
	return 0;
}