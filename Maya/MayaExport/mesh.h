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
	MStatus exportMesh(MFnMesh& mesh, map<string, unsigned int>& materials, meshStruct& meshes);

private:
	MStatus exportMaterial(MFnMesh& mesh, map<string, unsigned int>& materials);
	MStatus exportVertices(MFnMesh& mesh, meshStruct& meshes);
	MStatus exportJoints(MFnMesh& mesh);
};

#endif