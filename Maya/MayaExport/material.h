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
	MStatus exportMaterial(vector<Material>& mat_vector, map<const char*, int>& mat_map);
private:
	MStatus commonDiffuseValues(MFnDependencyNode& node, Material& matStrct);
	MStatus commonReflectValues(MFnDependencyNode& node, Material& matStrct);
	MStatus findTextures(MFnDependencyNode& node, Material& matStrct);

	MPlug plug;
};

#endif /* MATERIAL_H */