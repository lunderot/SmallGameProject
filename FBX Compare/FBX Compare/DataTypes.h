#ifndef DATATYPES_H
#define DATATYPES_H

#include <vector>
#include <iostream>
#include <set>
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
	FbxDouble3 color;
	FbxDouble3 shadow_color;
	double intensity;
	unsigned int light_type;
	unsigned int decay_type;
	bool cast_shadows;
};

struct MaterialInfo
{
	FbxDouble3 specular;
	FbxDouble3 reflection;
	FbxDouble3 ambient;
	FbxDouble3 diffuse;
	FbxDouble3 transparency_color;
	FbxDouble3 incandescence;
	double specular_factor;
	double shininess;
	double reflection_factor;
	double diffuse_factor;
	unsigned int material_type;
};

#endif /* DATATYPES_H */