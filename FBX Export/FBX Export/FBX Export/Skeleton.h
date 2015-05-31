#pragma once
#include <fbxsdk.h>
#include <iostream>
#include <vector>
#pragma comment (lib, "libfbxsdk.lib")
#include "Importer.h"
class Skeleton
{
public:
	Skeleton();
	~Skeleton();
	void ExportSkeletons(FbxScene* scene, std::string fileName);
};