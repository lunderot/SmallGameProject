#pragma once
#include <fbxsdk.h>
#include <iostream>
#include <vector>
#pragma comment (lib, "libfbxsdk.lib")
#include "Importer.h"
#include "Material.h"
class Mesh
{
public:
	void ExportMeshes(FbxScene* scene, std::string fileName);
	~Mesh();
};