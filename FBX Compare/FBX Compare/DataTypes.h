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

struct CamInfo
{
	FbxDouble3 position;
	FbxDouble3 up_vector;
	FbxDouble3 interest_position;
	double field_of_view_x;
	double field_of_view_y;
	double near_plane;
	double far_plane;
	unsigned int projection;
};

struct LightInfo
{
	unsigned int light_type;
	FbxDouble3 color;
	double intensity;
	unsigned int decay_type;
	bool cast_shadows;
	FbxDouble3 shadow_color;
};

#endif /* DATATYPES_H */