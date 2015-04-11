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
	MStatus exportMesh(MFnMesh& mesh, map<string, unsigned int>& materials, ofstream& outputFile);

private:
	MStatus exportMaterial(MFnMesh& mesh, map<string, unsigned int>& materials, ofstream& outputFile);
	MStatus exportVertices(MFnMesh& mesh, ofstream& outputFile);
	MStatus exportJoints(MFnMesh& mesh, ofstream& outputFile);
};

#endif