#include "exporter.h"
#include "mesh.h"
#include "maya_includes.h"
//--Ayu
#include "Light.h"

#include <iostream>

MStatus Exporter::doIt(const MArgList& argList)
{
	MDagPath path;
	MItDag dagIt(MItDag::kBreadthFirst);

	Mesh mesh;
	map<string, unsigned int> materials;
	ofstream outfile("J://Litet spelproj//test.bin", ofstream::binary);

	//--Ayu
	exportLight aLight; //--

	while (!dagIt.isDone())
	{
		if (dagIt.getPath(path))
		{
			if (path.apiType() == MFn::kMesh)
			{
				MFnMesh mayaMesh(path);
				mesh.exportMesh(mayaMesh, materials, outfile);
			}
			//--ayu
			// && !path.hasFn(MFn::defaultlight
			if (path.hasFn(MFn::kLight))
			{
				LightHeader eLHeader;
				Light eOLight;

				MFnLight eMayaLight(path);
				aLight.exportLightType(eMayaLight, eLHeader, eOLight);
			} // ---
			
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