#ifndef MATERIAL_H
#define MATERIAL_H

#include "maya_includes.h"
#include <maya/MPlug.h>

#include "CommonDeclaration.h"
#include <fstream>
#include <string>

using namespace std;

class Materials
{
public:
	MStatus exportMaterial(ofstream& outputFile);
private:
	MStatus commonDiffuseValues(MFnDependencyNode& node, struct Material& matStrct, struct MaterialHeader& matHead);
	MStatus commonReflectValues(MFnDependencyNode& node, struct Material& matStrct);
	MStatus findTextures(MFnDependencyNode& node, struct Material& matStrct, struct MaterialHeader& matHead);

	MPlug plug;
};

#endif /* MATERIAL_H */