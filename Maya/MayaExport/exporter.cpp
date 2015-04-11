#include "exporter.h"
#include "mesh.h"
#include "material.h"
#include "Transform.h"
#include "maya_includes.h"
#include "writeToFile.h"

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
	Header header;

	vector<TransformHeader> transfromHeaders;
	vector<Transform> transformData;

	while (!dagIt.isDone())
	{
		if (dagIt.getPath(path))
		{
			if (path.apiType() == MFn::kMesh)
			{
				MFnMesh mayaMesh(path);
				//status = mesh.exportMesh(mayaMesh, materials, outfile);
				header.mesh_count++;
			}
			if (path.apiType() == MFn::kTransform)
			{
				MStatus status;
				TransformHeader transformHeader;
				Transform transform;

				TransformClass transformClass;

				MFnTransform mayaTransform(path.node(), &status);
				status = transformClass.exportTransform(mayaTransform, heiraki, transfromHeaders.size(), transformHeader, transform);
				if (status != MS::kSuccess)
				{
					MGlobal::displayInfo("Failure at TransformClass::exportTransform()");
				}
				transfromHeaders.push_back(transformHeader);
				transformData.push_back(transform);
				header.group_count++;
			}
			if (path.apiType() == MFn::kWeightGeometryFilt)
			{
				
			}
		}
		dagIt.next(); // without this line, Maya will crash.
	}

	//Printing to files
	WriteToFile output;
	output.binaryFilePath("c://test.bin");
	output.ASCIIFilePath("c://testASCII.txt");
	output.OpenFiles();
	output.writeToFiles(&header, 1);
	output.writeToFiles(&transfromHeaders[0], transfromHeaders.size());
	output.writeToFiles(&transformData[0], transformData.size());

	output.CloseFiles();
	return MStatus::kSuccess;
}

void* Exporter::creator()
{
	return new Exporter;
}