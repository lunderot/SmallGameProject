#pragma once
#include <fbxsdk.h>
#include <iostream>
#include <vector>
#pragma comment (lib, "libfbxsdk.lib")
#include "Importer.h"

class Material
{
public:
	void ExportMaterials(FbxScene* scene, FbxMesh* mesh, const ImporterMesh &importedMesh, const ImporterMaterial* importedMaterials);

};