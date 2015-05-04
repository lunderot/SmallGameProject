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
	MStatus exportMaterial(
		vector<MaterialData>& mat_vector, 
		map<const char*, int>& mat_map, 
		std::string output_dir, 
		vector<const char*> &node_name, 
		vector<const char*> &diffuse_map,
		vector<const char*> &normal_map,
		vector<const char*> &specular_map);
private:
	MStatus commonDiffuseValues(
		MFnDependencyNode& node, 
		MaterialData& matStrct,
		vector<const char*> &node_name,
		vector<const char*> &diffuse_map,
		vector<const char*> &normal_map,
		vector<const char*> &specular_map);
	MStatus commonReflectValues(
		MFnDependencyNode& node, 
		MaterialData& matStrct,
		vector<const char*> &node_name,
		vector<const char*> &diffuse_map,
		vector<const char*> &normal_map,
		vector<const char*> &specular_map);
	MStatus findTextures(
		MFnDependencyNode& node, 
		MaterialData& matStrct,
		vector<const char*> &node_name,
		vector<const char*> &diffuse_map,
		vector<const char*> &normal_map,
		vector<const char*> &specular_map);

	MPlug plug;
	std::string outputDir;
};

#endif /* MATERIAL_H */