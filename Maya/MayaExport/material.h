#ifndef MATERIAL_H
#define MATERIAL_H

#include "maya_includes.h"
#include <maya/MPlug.h>

#include <string>
#include <vector>
#include <map>
#include "CommonDeclaration.h"

using namespace std;

class Materials
{
public:
	//Johan har ändrat lite i input
	MStatus exportMaterial(vector<struct MaterialHeader>& mat_head_vector, vector<struct Material>& mat_vector, map<const char*, unsigned int> mat_map);
private:
	MStatus commonDiffuseValues(MFnDependencyNode& node, struct Material& matStrct, struct MaterialHeader& matHead);
	MStatus commonReflectValues(MFnDependencyNode& node, struct Material& matStrct);
	MStatus findTextures(MFnDependencyNode& node, struct Material& matStrct, struct MaterialHeader& matHead);

	MPlug plug;
};

#endif /* MATERIAL_H */