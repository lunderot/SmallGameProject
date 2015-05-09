#ifndef DATATYPES_H
#define DATATYPES_H

#include <vector>
#include <iostream>
#include <fbxsdk.h>

struct MeshInfo
{
	std::vector<FbxVector4> normals;
	std::vector<FbxVector4> position;
};

#endif /* DATATYPES_H */