#ifndef MESH_H
#define MESH_H

#include "maya_includes.h"
#include "CommonDeclaration.h"
#include <fstream>
#include <map>

using namespace std;

class Mesh
{
public:
	MStatus exportMesh(MFnMesh& mesh, map<const char*, unsigned int>& materials, map<const char*, int> transformHeiraki, meshStruct& meshes, MeshHeader& mayaMeshHeader, map<const char*, unsigned int> meshMap);

private:
	MStatus exportMaterial(MFnMesh& mesh, map<const char*, unsigned int>& materials, meshStruct& meshes, MeshHeader& mayaMeshHeader);
	MStatus exportVertices(MFnMesh& mesh, map<const char*, int> transformHeiraki, map<const char*, unsigned int>& materials, meshStruct& meshes, MeshHeader& mayaMeshHeader);
	MStatus exportJoints(MFnMesh& mesh);
};

#endif