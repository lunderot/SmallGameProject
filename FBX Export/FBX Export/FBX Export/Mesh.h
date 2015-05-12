#pragma once
#include <fbxsdk.h>
#include <iostream>
#include <vector>
#pragma comment (lib, "libfbxsdk.lib")
#include "Importer.h"
class Mesh
{
public:
	FbxMesh* GetMesh(FbxScene* scene, std::string fileName);
	~Mesh();
};