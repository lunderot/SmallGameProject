#include "exporter.h"
#include "mesh.h"
#include "maya_includes.h"
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MFnAnimCurve.h>


#include "Light.h"
#include "Nurb.h"

#define MAYA_EXPORT
#include "CommonDeclaration.h"
#include "material.h"
#include "Transform.h"
#include "camera.h"
#include "JointExporter.h"
#include "morphAnimation.h"
#include "Keyframe.h"
#include "SkeletonAnimation.h"
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
	vector<Light> lightbody;

	//--Nurb
	exportNurb aNurb;
	vector<Nurb> nurbBody;

	map<const char*, int> materials;
	map<const char*, int> transformHeiraki;
	map<const char*, int> jointHeiraki;
	map<const char*, unsigned int> meshMap;
	Header header;
	vector<Material> mat;
	Materials  matExporter;
	matExporter.exportMaterial(mat, materials);
	header.material_count = mat.size();

	vector<Transform> transformData;

	vector<Joint> joints;

	// camera
	Camera cam;
	vector<camera> cameraVec;

	vector <meshStruct> meshes;

	MorphAnimations morphAnims;
	vector <MorphAnimation> morphs;

	Keyframe keyFrame;
	SkeletonAnimation skelAnim;
	vector<Keyframes> keysStorage;
	vector<SkinAnimation> skinStorage;

	MDagPath testdag;

	for (; !dagIt.isDone(); dagIt.next())
	{
		if (dagIt.getPath(path))
		{
			if (path.apiType() == MFn::kMesh)
			{
				MStatus status;
				meshStruct newMesh;
				MFnMesh mayaMesh(path);

				status = mesh.exportMesh(mayaMesh, materials, transformHeiraki, newMesh, meshMap);
				if (status != MS::kSuccess)
				{
					MGlobal::displayInfo("Failure at newMesh::exportMesh()");
					return status;
				}

				meshes.push_back(newMesh);
				header.mesh_count++;
			}
			if (path.apiType() == MFn::kTransform)
			{
				//MStatus status;
				Transform transform;

				TransformClass transformClass;

				MFnTransform mayaTransform(path.node(), &status);
				status = transformClass.exportTransform(mayaTransform, transformHeiraki, transformData.size(), transform);
				if (status != MS::kSuccess)
				{
					MGlobal::displayInfo("Failure at TransformClass::exportTransform()");
					return status;
				}
				transformData.push_back(transform);
				header.group_count++;
			}
			if (path.apiType() == MFn::kCamera)
			{
				MFnCamera mayaCamera(path);
				camera camera;
				status = cam.exportCamera(mayaCamera, camera, transformHeiraki);

				if (status != MS::kSuccess)
				{
					MGlobal::displayInfo("Failure at Camera::exportCamera()");
					return status;
				}
				cameraVec.push_back(camera);
				header.camera_count++;
			}
			if (path.apiType() == MFn::kJoint)
			{
				MFnIkJoint mayaJoint(path);

				Joint joint;

				JointExporter jointExporter;
				status = jointExporter.exportJoint(mayaJoint, jointHeiraki, transformHeiraki, joints.size(), joint);
				if (status != MS::kSuccess)
				{
					MGlobal::displayInfo("Failure at JointExporter::exportJoint()");
					return status;
				}

				joints.push_back(joint);
				header.joint_count++;
			}

			//--ayu
			// && !path.hasFn(MFn::defaultlight
			if (path.hasFn(MFn::kNonAmbientLight))
			{
				MS status;
				Light eOLight;

				//MFnLight eMayaLight(path);
				//MFnNonAmbientLight eMayaLight(path);
				MObject eMayaLight = path.node();
				status = aLight.exportLightType(eMayaLight, eOLight);

			} // ---

			//NurbSurface
			if (path.hasFn(MFn::kNurbsSurface))
			{
				MS status;
				Nurb structNurb;

				//MFnNurbsSurface eNurb(path);
				//aNurb.exportNurbSphere(eNurb, nurbHeader, structNurb, transformHeiraki);

				MFnNurbsSurface eNurb = path.node();
				status = aNurb.exportNurbSphere(eNurb, structNurb, transformHeiraki);

				MGlobal::displayInfo("NURB FINNS");

				if (status == MS::kSuccess)
				{;
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
		MorphAnimation morphAnim;
		MObject testing = it.item();
		MFnDagNode storetest(testing);
		MFnDependencyNode wuut(testing);
		MFnAnimCurve anim(testing, &status);

		morphAnims.exportMorphAnimation(it, morphAnim, meshMap);
		morphs.push_back(morphAnim);

		it.next();
	}


	MGlobal::displayInfo(MString() + "=============================================");
	// Skin animation
	MItDependencyNodes skinLoop(MFn::kSkinClusterFilter);

	while (!skinLoop.isDone())
	{
		MS status = MS::kSuccess;
		SkinAnimation skin;

		MObject animSkin = skinLoop.item();

		skelAnim.exportSkin(animSkin, skin, jointHeiraki, meshMap);
		//for (int c = 0; c < skin.skinVertexCount; c++)
		//	MGlobal::displayInfo(MString() + skin.influenceWeights[c].influenceObject[0] + " " + skin.influenceWeights[c].weight[0]);

		skinStorage.push_back(skin);

		skinLoop.next();
	}

	// Keyframes
	MItDependencyNodes curveLoop(MFn::kAnimCurve);

	while (!curveLoop.isDone())
	{
		MS status = MS::kSuccess;
		Keyframes key;

		MObject animCurve = curveLoop.item();

		status = keyFrame.exportKeyframes(animCurve, key, transformHeiraki, jointHeiraki);

		if (status == MS::kSuccess)
		{
			keysStorage.push_back(key);
		}

		curveLoop.next();
	}

	//Printing to files
	WriteToFile output;
	output.binaryFilePath("C://test.bin");
	output.ASCIIFilePath("C://testASCII.txt");
	output.OpenFiles();

	//Main header
	output.writeToFiles(&header, 1);

	//Data
	output.writeToFiles(&mat[0], mat.size());
	output.writeToFiles(&transformData[0], transformData.size());
	output.writeToFiles(&joints[0], joints.size());
	output.writeToFiles(&cameraVec[0], cameraVec.size());
	output.writeToFiles(&meshes[0], meshes.size());
	output.writeToFiles(&lightbody[0], lightbody.size());
	output.writeToFiles(&nurbBody[0], nurbBody.size());
	output.writeToFiles(&morphs[0], morphs.size());
	output.writeToFiles(&skinStorage[0], skinStorage.size());
	output.writeToFiles(&keysStorage[0], keysStorage.size());

	output.CloseFiles();
	return MStatus::kSuccess;

}

void* Exporter::creator()
{
	return new Exporter;
}