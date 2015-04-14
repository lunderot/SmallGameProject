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
#include "morphAnimation.h"
#include "writeToFile.h"

#include <iostream>
#include <vector>
#include <map>

using namespace std;

MStatus Exporter::doIt(const MArgList& argList)
{
	MStatus status;

	MString cmdAll = "select -all";
	MGlobal::executeCommand(cmdAll);
	MString cmdF = "polyEvaluate -f";
	MIntArray faces;
	MGlobal::executeCommand(cmdF, faces, true, false);
	cmdF = "polyEvaluate -t";
	MIntArray triangles;
	MGlobal::executeCommand(cmdF, triangles, true, false);
	if (faces[0] == triangles[0])
	{

		MDagPath path;
		MItDag dagIt(MItDag::kBreadthFirst);

		Mesh mesh;

		//map<const char*, unsigned int> materials;
		ofstream outfile("J://Litet spelproj//test.bin", ofstream::binary);

		//--Ayu
		exportLight aLight; //--
		vector<LightHeader> lighthead;
		vector<Light> lightbody;


		map<const char*, unsigned int> materials;
		map<const char*, int> transformHeiraki;
		map<const char*, int> jointHeiraki;
		Header header;

		vector<MaterialHeader> mat_headers;
		vector<Material> mat;
		Materials  matExporter;
		matExporter.exportMaterial(mat_headers, mat, materials);
		header.material_count = mat_headers.size();

		vector<TransformHeader> transformHeaders;
		vector<Transform> transformData;

		vector<JointHeader> jointHeaders;
		vector<Joint> joints;

		// camera
		Camera cam;
		vector<CameraHeader> camera_header;
		vector<camera> cameraVec;

		vector <MeshHeader> meshHeader;
		vector <meshStruct> meshes;

		MorphAnimations morphAnims;
		vector<morphAnimationHeader> morphHeader;
		vector <MorphAnimation> morphs;

		MDagPath testdag;

		while (!dagIt.isDone())
		{
			if (dagIt.getPath(path))
			{
				if (path.apiType() == MFn::kMesh)
				{
					MStatus status;
					meshStruct newMesh;
					MeshHeader mayaMeshHeader;
					MFnMesh mayaMesh(path);

					status = mesh.exportMesh(mayaMesh, materials, transformHeiraki, newMesh, mayaMeshHeader);
					if (status != MS::kSuccess)
					{
						MGlobal::displayInfo("Failure at newMesh::exportMesh()");
					}
					meshHeader.push_back(mayaMeshHeader);
					meshes.push_back(newMesh);
					header.mesh_count++;
				}
				if (path.apiType() == MFn::kTransform)
				{
					//MStatus status;
					TransformHeader transformHeader;
					Transform transform;

					TransformClass transformClass;

					MFnTransform mayaTransform(path.node(), &status);
					status = transformClass.exportTransform(mayaTransform, transformHeiraki, transformHeaders.size(), transformHeader, transform);
					if (status != MS::kSuccess)
					{
						MGlobal::displayInfo("Failure at TransformClass::exportTransform()");
						return status;
					}
					transformHeaders.push_back(transformHeader);
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
					header.camera_count++;
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
				if (path.hasFn(MFn::kNonAmbientLight))
				{
					LightHeader eLHeader;
					Light eOLight;

					//MFnLight eMayaLight(path);
					//MFnNonAmbientLight eMayaLight(path);
					MObject eMayaLight = path.node();
					aLight.exportLightType(eMayaLight, eLHeader, eOLight);

					lighthead.push_back(eLHeader);
					lightbody.push_back(eOLight);
				} // ---

				if (path.hasFn(MFn::kBlendShape))
				{
					morphAnimationHeader morphHead;
					MorphAnimation morphAnim;
					cout << "ALLAHU AKBAR" << endl;

					//morphAnims.exportMorphAnimation();


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
		output.writeToFiles(mat_headers.data(), mat_headers.size());
		output.writeToFiles(transformHeaders.data(), transformHeaders.size());
		output.writeToFiles(jointHeaders.data(), jointHeaders.size());
		output.writeToFiles(camera_header.data(), camera_header.size());
		output.writeToFiles(meshHeader.data(), meshHeader.size());

		//Data
		output.writeToFiles(&mat[0], &mat_headers[0], mat.size());
		output.writeToFiles(&transformData[0], &transformHeaders[0], transformData.size());
		output.writeToFiles(&joints[0], &jointHeaders[0], joints.size());
		output.writeToFiles(&cameraVec[0], &camera_header[0], cameraVec.size());
		output.writeToFiles(&meshes[0], &meshHeader[0], meshHeader.size());

		output.CloseFiles();
		return MStatus::kSuccess;
	}
	else
	{
		MGlobal::displayError("TRIANGULATE IT PLZ. motherfucker #yolo #swag");
		return MStatus::kFailure;
	}
}

void* Exporter::creator()
{
	return new Exporter;
}