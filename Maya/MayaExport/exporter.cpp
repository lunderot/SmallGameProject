#include "exporter.h"
#include "mesh.h"
#include "material.h"
#include "Transform.h"
#include "camera.h"
#include "JointExporter.h"
#include "writeToFile.h"

#include <vector>
#include <map>

MStatus Exporter::doIt(const MArgList& argList)
{
	MStatus status;

	MDagPath path;
	MItDag dagIt(MItDag::kBreadthFirst);

	Mesh mesh;
	map<const char*, unsigned int> materials;
	map<const char*, int> transformHeiraki;
	map<const char*, int> jointHeiraki;
	Header header;

	vector<MaterialHeader> mat_headers;
	vector<Material> mat;
	Materials  matExporter;
	matExporter.exportMaterial(mat_headers, mat, materials);
	header.material_count = mat_headers.size();

	vector<TransformHeader> transfromHeaders;
	vector<Transform> transformData;

	vector<JointHeader> jointHeaders;
	vector<Joint> joints;

	// camera
	Camera cam;
	vector<CameraHeader> camera_header;
	vector<camera> cameraVec;

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
				status = transformClass.exportTransform(mayaTransform, transformHeiraki, transfromHeaders.size(), transformHeader, transform);
				if (status != MS::kSuccess)
				{
					MGlobal::displayInfo("Failure at TransformClass::exportTransform()");
					return status;
				}
				transfromHeaders.push_back(transformHeader);
				transformData.push_back(transform);
				header.group_count++;
			}
			if (path.apiType() == MFn::kCamera)
			{
				MFnCamera mayaCamera(path);
				camera camera;
				CameraHeader camHeader;
				status = cam.exportCamera(mayaCamera, camera, camHeader);

				if (status != MS::kSuccess)
				{
					MGlobal::displayInfo("Failure at Camera::exportCamera()");
				}
				camera_header.push_back(camHeader);
				cameraVec.push_back(camera);
			}
			if (path.apiType() == MFn::kJoint)
			{
				MFnIkJoint mayaJoint(path);

				Joint joint;
				JointHeader jointHeader;

				JointExporter jointExporter;
				status = jointExporter.exportJoint(mayaJoint, jointHeiraki, transformHeiraki, joints.size(), jointHeader, joint);
				if (status != MS::kSuccess)
				{
					MGlobal::displayInfo("Failure at JointExporter::exportJoint()");
					return status;
				}

				jointHeaders.push_back(jointHeader);
				joints.push_back(joint);
			}
			
		}
		dagIt.next(); // without this line, Maya will crash.
	}

	//Printing to files
	WriteToFile output;
	output.binaryFilePath("C://test.bin");
	output.ASCIIFilePath("C://testASCII.txt");
	output.OpenFiles();

	//Main header
	output.writeToFiles(&header, 1);

	//Headers
	output.writeToFiles(&mat_headers[0], mat_headers.size());
	output.writeToFiles(&transfromHeaders[0], transfromHeaders.size());
	output.writeToFiles(&jointHeaders[0], jointHeaders.size());

	//Data
	output.writeToFiles(&mat[0], mat.size());
	output.writeToFiles(&transformData[0], transformData.size());
	output.writeToFiles(&joints[0], joints.size());

	// camera
	output.writeToFiles(&cameraVec[0], cameraVec.size());

	output.CloseFiles();
	return MStatus::kSuccess;
}

void* Exporter::creator()
{
	return new Exporter;
}