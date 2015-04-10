#include "exporter.h"
#include "mesh.h"
#include "Transform.h"
#include "maya_includes.h"

#include <iostream>
#include <vector>
#include <map>

MStatus Exporter::doIt(const MArgList& argList)
{
	MStatus status;

	MDagPath path;
	MItDag dagIt(MItDag::kBreadthFirst);

	Mesh mesh;
	map<string, unsigned int> materials;
	map<const char*, int> heiraki;

	vector<TransformHeader> transfromHeaders;
	vector<Transform> transformData;

	ofstream outfile("c://test.bin", ofstream::binary);
	ofstream outfileASCII("c://testASCII.txt");
	while (!dagIt.isDone())
	{
		if (dagIt.getPath(path))
		{
			if (path.apiType() == MFn::kMesh)
			{
				MFnMesh mayaMesh(path);
				status = mesh.exportMesh(mayaMesh, materials, outfile);
			}
			if (path.apiType() == MFn::kTransform)
			{
				MStatus status;
				TransformHeader transformHeader;
				Transform transform;

				TransformClass transformClass;

				MFnTransform mayaTransform(path.node(), &status);
				MGlobal::displayInfo(MString() + status);
				MGlobal::displayInfo(path.fullPathName());
				status = transformClass.exportTransform(mayaTransform, heiraki, transfromHeaders.size(), transformHeader, transform);
				if (status != MS::kSuccess)
				{
					MGlobal::displayInfo("Failure at TransformClass::exportTransform()");
				}
				transfromHeaders.push_back(transformHeader);
				transformData.push_back(transform);

				//Kod för att se om det kan skrivas till fil
				outfile.write( (const char*)&transformHeader, sizeof(TransformHeader));
				outfile.write((const char*)&transform, sizeof(Transform));
				outfileASCII << transformHeader.name_Length << endl;
				for (unsigned int i = 0; i < transformHeader.name_Length; i++)
					outfileASCII << transform.name[i];

				outfileASCII << endl << transform.parentID << endl << transform.position[0] << ' ' << transform.position[1] << ' ' << transform.position[3] << endl
					<< transform.rotation[0] << ' ' << transform.rotation[1] << ' ' << transform.rotation[2] << ' ' << transform.rotation[3] << endl
					<< transform.scale[0] << ' ' << transform.scale[1] << ' ' << transform.scale[2] << endl << endl;
			}
		}
		dagIt.next(); // without this line, Maya will crash.
	}
	outfile.close();
	outfileASCII.close();
	return MStatus::kSuccess;
}

void* Exporter::creator()
{
	return new Exporter;
}