#include "exporter.h"
#include "mesh.h"
#include "maya_includes.h"
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MFnAnimCurve.h>


#include "Light.h"
#include "Nurb.h"


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

	MDagPath path;
	MItDag dagIt(MItDag::kBreadthFirst);
	Mesh mesh;

	//map<const char*, unsigned int> materials;
	//ofstream outfile("J://Litet spelproj//test.bin", ofstream::binary);

	//--Light
	exportLight aLight;
	vector<LightHeader> lighthead;
	vector<Light> lightbody;

	//--Nurb
	exportNurb aNurb;
	vector<NurbHeader> nurbHead;
	vector<Nurb> nurbBody;

	map<const char*, int> materials;
	map<const char*, int> transformHeiraki;
	map<const char*, int> jointHeiraki;
	map<const char*, unsigned int> meshMap;
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

	for (; !dagIt.isDone(); dagIt.next())
	{
		if (dagIt.getPath(path))
		{
			if (path.apiType() == MFn::kMesh)
			{
				MStatus status;
				meshStruct newMesh;
				MeshHeader mayaMeshHeader;
				MFnMesh mayaMesh(path);

				status = mesh.exportMesh(mayaMesh, materials, transformHeiraki, newMesh, mayaMeshHeader, meshMap);
				if (status != MS::kSuccess)
				{
					MGlobal::displayInfo("Failure at newMesh::exportMesh()");
					return status;
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
				status = cam.exportCamera(mayaCamera, camera, camHeader, transformHeiraki);

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
				header.joint_count++;
			}

			//--ayu
			// && !path.hasFn(MFn::defaultlight
			if (path.hasFn(MFn::kNonAmbientLight))
			{
				MS status;
				LightHeader eLHeader;
				Light eOLight;

				//MFnLight eMayaLight(path);
				//MFnNonAmbientLight eMayaLight(path);
				MObject eMayaLight = path.node();
				status = aLight.exportLightType(eMayaLight, eLHeader, eOLight);

			} // ---

			//NurbSurface
			if (path.hasFn(MFn::kNurbsSurface))
			{
				MS status;
				NurbHeader nurbHeader;
				Nurb structNurb;

				//MFnNurbsSurface eNurb(path);
				//aNurb.exportNurbSphere(eNurb, nurbHeader, structNurb, transformHeiraki);

				MFnNurbsSurface eNurb = path.node();
				status = aNurb.exportNurbSphere(eNurb, nurbHeader, structNurb, transformHeiraki);

				MGlobal::displayInfo("NURB FINNS");

				if (status == MS::kSuccess)
				{
					nurbHead.push_back(nurbHeader);
					nurbBody.push_back(structNurb);
					header.nurb_count++;
				}

			}

		}
	
	}
	// Blend Shapes
	MItDependencyNodes it(MFn::kBlendShape);

	while (!it.isDone())
	{
		morphAnimationHeader morphHead;
		MorphAnimation morphAnim;
		MObject testing = it.item();
		MFnDagNode storetest(testing);
		MFnDependencyNode wuut(testing);
		MFnAnimCurve anim(testing, &status);
		
		//MGlobal::displayInfo(MString() + "FAKAKAKFAKFAKAKFAKFA: " + );
		MGlobal::displayInfo(MString() + "nanananananame: " + anim.parentNamespace().asChar());
		MGlobal::displayInfo(MString() + "nanananananame: " + wuut.name());
		MGlobal::displayInfo(MString() + "nanananananame: " + MString() + anim.numKeys());

		morphAnims.exportMorphAnimation(it, morphHead, morphAnim, meshMap);
		morphHeader.push_back(morphHead);
		morphs.push_back(morphAnim);

		it.next();
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
	output.writeToFiles(lighthead.data(), lighthead.size());
	output.writeToFiles(nurbHead.data(), nurbHead.size());
	//output.writeToFiles(morphHeader.data(), morphHeader.size());

	//Data
	output.writeToFiles(&mat[0], &mat_headers[0], mat.size());
	output.writeToFiles(&transformData[0], &transformHeaders[0], transformData.size());
	output.writeToFiles(&joints[0], &jointHeaders[0], joints.size());
	output.writeToFiles(&cameraVec[0], &camera_header[0], cameraVec.size());
	output.writeToFiles(&meshes[0], &meshHeader[0], meshes.size());
	output.writeToFiles(&lightbody[0], &lighthead[0], lightbody.size());
	output.writeToFiles(&nurbBody[0], &nurbHead[0], nurbBody.size());
	//output.writeToFiles(&morphs[0], &morphHeader[0], morphs.size());

	output.CloseFiles();
	return MStatus::kSuccess;

}

void* Exporter::creator()
{
	return new Exporter;
}