#include "exporter.h"
#include "mesh.h"
#include "maya_includes.h"

#include <iostream>

MStatus Exporter::doIt(const MArgList& argList)
{
	MDagPath path;
	MItDag dagIt(MItDag::kBreadthFirst);

	Mesh mesh;
	map<string, unsigned int> materials;
	ofstream outfile("test.bin", ofstream::binary);

	while (!dagIt.isDone())
	{
		if (dagIt.getPath(path))
		{
			if (path.apiType() == MFn::kMesh)
			{
				MFnMesh mayaMesh(path);
				mesh.exportMesh(mayaMesh, materials, outfile);
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