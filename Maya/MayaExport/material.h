#ifndef MATERIAL_H
#define MATERIAL_H
#include "maya_includes.h"
#include <maya/MPlug.h>

#include <string>
#include <vector>
#include <map>
#define MAYA_EXPORT
#include "CommonDeclaration.h"

class Materials
{
public:
	MStatus exportMaterial(vector<Material>& mat_vector, map<const char*, int>& mat_map, std::string output_dir);
private:
	MStatus commonDiffuseValues(MFnDependencyNode& node, Material& matStrct);
	MStatus commonReflectValues(MFnDependencyNode& node, Material& matStrct);
	MStatus findTextures(MFnDependencyNode& node, Material& matStrct);

	MPlug plug;
	std::string outputDir;
};

#endif /* MATERIAL_H */