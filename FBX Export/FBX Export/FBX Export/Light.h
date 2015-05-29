#pragma once
#include <fbxsdk.h>
#include <iostream>
#include <vector>
#pragma comment (lib, "libfbxsdk.lib")
#include "Importer.h"

class Light
{
public:
	void ExportLights(FbxScene* scene, std::string fileName);

};