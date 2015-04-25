#ifndef MESH_H
#define MESH_H

#include "maya_includes.h"
#define MAYA_EXPORT
#include "CommonDeclaration.h"
#include <fstream>
#include <map>

using namespace std;

class Mesh
{
public:
	MStatus exportMesh(MFnMesh& mesh, map<const char*, int>& materials, map<const char*, int>& transformHeiraki, meshStruct& meshes, map<const char*, unsigned int>& meshMap);

private:
	MStatus exportMaterial(MFnMesh& mesh, map<const char*, int>& materials, meshStruct& meshes);
	MStatus exportVertices(MFnMesh& mesh, map<const char*, int>& transformHeiraki, map<const char*, int>& materials, meshStruct& meshes);
	MStatus exportJoints(MFnMesh& mesh);
};

#endif