#include "exporter.h"
#include "mesh.h"
#include "maya_includes.h"

//--Ayu
#include "Light.h"


#include "CommonDeclaration.h"
#include "material.h"
#include "Transform.h"
#include "camera.h"
#include "JointExporter.h"
#include "writeToFile.h"

#include <iostream>
#include <vector>
#include <map>

using namespace std;

MStatus Exporter::doIt(const MArgList& argList)
{
	MStatus status;

	MDagPath path;
	MItDag dagIt(MItDag::kBreadthFirst);

	Mesh mesh;

	//map<const char*, unsigned int> materials;
	ofstream outfile("J://Litet spelproj//test.bin", ofstream::binary);

	//--Ayu
	exportLight aLight; //--

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
					return status;
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
	output.writeToFiles(&camera_header[0], camera_header.size());

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