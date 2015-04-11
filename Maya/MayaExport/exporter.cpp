#include "exporter.h"
#include "mesh.h"
#include "maya_includes.h"
#include "CommonDeclaration.h"

#include <iostream>
#include <vector>

using namespace std;

MStatus Exporter::doIt(const MArgList& argList)
{
	MDagPath path;
	MItDag dagIt(MItDag::kBreadthFirst);

	Mesh mesh;
	map<string, unsigned int> materials;
	ofstream outfile("test.bin", ofstream::binary);


	vector <meshStruct> meshes;

	while (!dagIt.isDone())
	{
		if (dagIt.getPath(path))
		{
			if (path.apiType() == MFn::kMesh)
			{
				meshStruct newMesh;
				MFnMesh mayaMesh(path);
				mesh.exportMesh(mayaMesh, materials, newMesh);
				meshes.push_back(newMesh);
			}
		}
		dagIt.next(); // without this line, Maya will crash.
	}
	outfile.close();
	return MStatus::kSuccess;
}

void* Exporter::creator()
{
	return new Exporter;
}