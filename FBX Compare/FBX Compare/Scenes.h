#ifndef SCENES_H
#define SCENES_H

#include <fbxsdk.h>
#include "Defines.h"
#include <string>

class Scenes
{
private:
	FbxScene* lSdkGoldenScene;
	FbxScene* lSdkTestScene;

	bool lSdkStatus = false;
	FbxImporter* lSdkImporter;
	void CheckStatus();
protected:
public:
	Scenes(FbxManager* pManager, std::string pGolden, std::string pTest);
	~Scenes();

	FbxScene* GetGolden();
	FbxScene* GetTest();
};

#endif /* SCENES_H */