#include "exporter.h"
#include "mesh.h"
#include "maya_includes.h"
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>


#include "Light.h"
#include "Nurb.h"

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
	MArgDatabase argData(newSyntax(), argList);

	if (argData.isFlagSet("-outputDir"))
	{
		MString tempString;
		argData.getFlagArgument("-outputDir", 0, tempString);
		this->outputDir = tempString.asChar();
	}
	else
	{
		this->outputDir = "";
	}

	if (argData.isFlagSet("-fileName"))
	{
		MString tempString;
		argData.getFlagArgument("-fileName", 0, tempString);
		this->fileName = tempString.asChar();
	}
	else
	{
		this->fileName = "";
	}

	MStatus status;

	MDagPath path;
	MItDag dagIt(MItDag::kBreadthFirst);
	Mesh mesh;

	//map<const char*, unsigned int> materials;
	//ofstream outfile("J://Litet spelproj//test.bin", ofstream::binary);

	//--Light
	exportLight aLight;
	vector<Light> lightbody;
	vector<const char*> expLightName;

	//--Nurb
	exportNurb aNurb;
	vector<Nurb> nurbBody;
	vector<const char*> expNurbName;
	vector<vector <int>> expParentID;

	map<const char*, int> materials;
	map<const char*, int> transformHeiraki;
	map<const char*, int> jointHeiraki;
	map<const char*, unsigned int> meshMap;
	Header header;
	vector<MaterialData> mat;
	Materials  matExporter;
	matExporter.exportMaterial(mat, materials, outputDir);
	header.material_count = mat.size();

	vector<const char*> transformNames;
	vector<Transform> transformData;

	vector<const char*> jointNames;
	vector<Joint> joints;

	// camera
	Camera cam;
	vector<camera> cameraVec;
	vector<const char*> camreNames;
	vector<vector<unsigned int>> cameraParentIDs;

	// mesh
	vector <meshStruct> meshes;
	vector<vector<double>> position;
	vector<vector<float>> uv;
	vector<vector<double>> normal;
	vector<vector<double>> tangent;
	vector<vector<double>> bi_tangent;

	vector<vector<int>> transform_Id;
	vector<vector<int>> material_Id;
	vector<vector<Vertex>> vertices;
	vector<const char*> name;


	MorphAnimations morphAnims;
	vector <MorphAnimation> morphs;
	vector<vector<double>> morphsPositions;
	vector <const char*> moprhsNames;

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

				status = mesh.exportMesh(mayaMesh, materials, transformHeiraki, newMesh, meshMap,
					position, uv, normal, tangent, bi_tangent, transform_Id, material_Id, vertices, name);
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
				status = transformClass.exportTransform(mayaTransform, transformHeiraki, transformData.size(), transform, transformNames);
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
				status = cam.exportCamera(mayaCamera, camera, transformHeiraki, camreNames, cameraParentIDs);

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
				status = jointExporter.exportJoint(mayaJoint, jointHeiraki, transformHeiraki, joints.size(), joint, jointNames);
				if (status != MS::kSuccess)
				{
					MGlobal::displayInfo("Failure at JointExporter::exportJoint()");
					return status;
				}

				joints.push_back(joint);
				header.joint_count++;
			}

			// && !path.hasFn(MFn::defaultlight
			if (path.hasFn(MFn::kNonAmbientLight))
			{
				MS status;
				Light eOLight;

				//MFnLight eMayaLight(path);
				//MFnNonAmbientLight eMayaLight(path);
				MObject eMayaLight = path.node();
				status = aLight.exportLightType(eMayaLight, eOLight, expLightName);
				if (status == MS::kSuccess)
				{
					lightbody.push_back(eOLight);
					header.light_count++;
				}

			} // ---

			//NurbSurface
			if (path.hasFn(MFn::kNurbsSurface))
			{
				MS status;
				Nurb structNurb;

				//MFnNurbsSurface eNurb(path);
				//aNurb.exportNurbSphere(eNurb, nurbHeader, structNurb, transformHeiraki);

				MFnNurbsSurface eNurb = path.node();
				status = aNurb.exportNurbSphere(eNurb, structNurb, transformHeiraki, expNurbName, expParentID);

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

		morphAnims.exportMorphAnimation(it, morphAnim, morphsPositions, moprhsNames, meshMap);
		morphs.push_back(morphAnim);
		header.morph_count++;

		it.next();
	}

	// Skin animation
	MItDependencyNodes skinLoop(MFn::kSkinClusterFilter);

	while (!skinLoop.isDone())
	{
		MS status = MS::kSuccess;
		SkinAnimation skin;

		MObject animSkin = skinLoop.item();

		skelAnim.exportSkin(animSkin, skin, jointHeiraki, meshMap);

		skinStorage.push_back(skin);

		header.skin_count++;

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
			header.anim_curve_count++;
		}

		curveLoop.next();
	}

	//Create and Open files
	WriteToFile output;
	output.binaryFilePath(this->outputDir + "/" + this->fileName + ".bin");
	output.ASCIIFilePath(this->outputDir + "/" + this->fileName + "ASCII.txt");
	output.OpenFiles();

	//Main header
	output.writeToFiles(&header);

	//Data
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		output.writeToFiles(position[i].data(), position[i].size());
		output.writeToFiles(uv[i].data(), uv[i].size());
		output.writeToFiles(normal[i].data(), normal[i].size());
		output.writeToFiles(tangent[i].data(), tangent[i].size());
		output.writeToFiles(bi_tangent[i].data(), bi_tangent[i].size());
		output.writeToFiles(transform_Id[i].data(), transform_Id[i].size());
		output.writeToFiles(material_Id[i].data(), material_Id[i].size());
		output.writeToFiles(vertices[i].data(), vertices[i].size());
		output.writeToFiles(&name[i]);
	}

	for (unsigned int i = 0; i < transformData.size(); i++)
	{
		output.writeToFiles(&transformData[i]);
		output.writeToFiles(&transformNames[i]);
	}

	for (unsigned int i = 0; i < joints.size(); i++)
	{
		output.writeToFiles(&joints[i]);
		output.writeToFiles(&jointNames[i]);
	}

	for (unsigned int i = 0; i < cameraVec.size(); i++)
	{
		output.writeToFiles(&cameraVec[i]);
		output.writeToFiles(&camreNames[i]);
	}

	/*output.writeToFiles(&meshes[0], meshes.size());*/

	for (unsigned int i = 0; i < lightbody.size(); i++)
	{
		output.writeToFiles(&expLightName[i]);
		output.writeToFiles(&lightbody[i]);
	}

	for (unsigned int i = 0; i < nurbBody.size(); i++)
	{
		output.writeToFiles(&expNurbName[i]);
		output.writeToFiles(&nurbBody[i]);
		//for (unsigned int j = 0; j < expParentID[i].size(); j++)
			output.writeToFiles(expParentID[i].data(), expParentID[i].size());
	}
	for (unsigned int i = 0; i < morphs.size(); i++)
	{
		output.writeToFiles(&morphs[i]);
		output.writeToFiles(morphsPositions[i].data(), morphsPositions[i].size());
		output.writeToFiles(&moprhsNames[i]);
	}
	//output.writeToFiles(&skinStorage[0], skinStorage.size());
	//output.writeToFiles(&keysStorage[0], keysStorage.size());

	output.CloseFiles();
	return MStatus::kSuccess;
}

MSyntax Exporter::newSyntax()
{
	MSyntax syntax;

	syntax.addFlag("-od", "-outputDir", MSyntax::kString);
	syntax.addFlag("-fn", "-fileName", MSyntax::kString);

	return syntax;
}

void* Exporter::creator()
{
	return new Exporter;
}