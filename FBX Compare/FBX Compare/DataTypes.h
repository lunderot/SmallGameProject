#ifndef DATATYPES_H
#define DATATYPES_H

#include <vector>
#include <iostream>
#include <fbxsdk.h>

struct MeshInfo
{
	std::vector<FbxVector4> normals;
	std::vector<FbxVector4> position;
	std::vector<FbxVector2> uv;
};

#endif /* DATATYPES_H */