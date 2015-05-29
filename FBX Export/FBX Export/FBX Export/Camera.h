#pragma once
#include <fbxsdk.h>
#include <iostream>
#include <vector>
#pragma comment (lib, "libfbxsdk.lib")
#include "Importer.h"

class Camera
{
public:
	void ExportCameras(FbxScene* scene, std::string fileName);
};