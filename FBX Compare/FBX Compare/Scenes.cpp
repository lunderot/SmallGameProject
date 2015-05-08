#include "Scenes.h"

Scenes::Scenes(FbxManager* pManager, std::string pGolden, std::string pTest)
{
	this->lSdkImporter = FbxImporter::Create(pManager, "Importer");

	this->lSdkGoldenScene = FbxScene::Create(pManager, "Golden");
	this->lSdkTestScene = FbxScene::Create(pManager, "Test");

	this->lSdkStatus = this->lSdkImporter->Initialize(pGolden.c_str(), -1, pManager->GetIOSettings());
	CheckStatus();
	this->lSdkImporter->Import(this->lSdkGoldenScene);

	this->lSdkStatus = this->lSdkImporter->Initialize(pTest.c_str(), -1, pManager->GetIOSettings());
	CheckStatus();
	this->lSdkImporter->Import(lSdkTestScene);
}

Scenes::~Scenes()
{
	//this->lSdkGoldenScene->Destroy();
	//this->lSdkTestScene->Destroy();
}

FbxScene* Scenes::GetGolden()
{
	return this->lSdkGoldenScene;
}

FbxScene* Scenes::GetTest()
{
	return this->lSdkTestScene;
}

void Scenes::CheckStatus()
{
	if (!this->lSdkStatus)
	{
		FBXSDK_printf("Failed to initialize importer.\n");
		FBXSDK_printf("Error: %s\n", lSdkImporter->GetStatus().GetErrorString());
		exit(-1);
	}
}