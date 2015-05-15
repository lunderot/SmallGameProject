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
	vector<LightData> lightbody;
	vector<const char*> expLightName;
	vector<vector<int>> expParentLightID;

	//--Nurb
	exportNurb aNurb;
	vector<NurbData> nurbBody;
	vector<const char*> expNurbName;
	vector<vector <int>> expParentID;

	map<const char*, int> materials;
	map<const char*, int> transformHeiraki;
	map<const char*, int> jointHeiraki;
	map<const char*, unsigned int> meshMap;
	Header header;

	vector<MaterialData> mat;
	Materials  matExporter;

	vector<const char*> node_name;
	vector<const char*> diffuse_map;
	vector<const char*> normal_map;
	vector<const char*> specular_map;

	matExporter.exportMaterial(mat, materials, outputDir, node_name, diffuse_map, normal_map, specular_map);
	header.material_count = mat.size();

	vector<const char*> transformNames;
	vector<TransformData> transformData;

	vector<const char*> jointNames;
	vector<JointData> joints;

	// camera
	Camera cam;
	vector<cameraData> cameraVec;
	vector<const char*> camreNames;
	vector<vector<unsigned int>> cameraParentIDs;

	// mesh
	vector <meshStructData> meshes;
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
	vector <MorphAnimationData> morphs;
	vector<vector<double>> morphsPositions;
	vector <const char*> moprhsNames;

	Keyframe keyFrame;
	vector<KeyframesData> keysStorage;
	vector<vector<KeyframePoint>> points;
	vector<const char*> curveName;
	vector<const char*> attachToName;

	SkeletonAnimation skelAnim;
	vector<SkinAnimation> skinStorage;
	vector<vector<int>> influenceIndices;
	vector<vector<VertexInfluence>> influenceWeights;

	MDagPath testdag;

	for (; !dagIt.isDone(); dagIt.next())
	{
		if (dagIt.getPath(path))
		{
			if (path.apiType() == MFn::kMesh)
			{
				MStatus status;
				meshStructData newMesh;
				MFnMesh mayaMesh(path);

				status = mesh.exportMesh(mayaMesh, materials, transformHeiraki, newMesh, meshMap, position, uv, normal, tangent, bi_tangent, transform_Id, material_Id, vertices, name);
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
				TransformData transform;

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
				cameraData camera;
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

				JointData joint;

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
				LightData eOLight;

				//MFnLight eMayaLight(path);
				//MFnNonAmbientLight eMayaLight(path);
				MObject eMayaLight = path.node();
				status = aLight.exportLightType(eMayaLight, eOLight, expLightName, transformHeiraki, expParentLightID);
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
				NurbData structNurb;

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
		MorphAnimationData morphAnim;
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

		skelAnim.exportSkin(animSkin, skin, jointHeiraki, meshMap, influenceIndices, influenceWeights);

		skinStorage.push_back(skin);

		header.skin_count++;

		skinLoop.next();
	}

	// Keyframes
	MItDependencyNodes curveLoop(MFn::kAnimCurve);

	while (!curveLoop.isDone())
	{
		MS status = MS::kSuccess;
		KeyframesData key;

		MObject animCurve = curveLoop.item();

		status = keyFrame.exportKeyframes(animCurve, key, transformHeiraki, jointHeiraki, points, curveName, attachToName);

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
	for (unsigned int i = 0; i < mat.size(); i++)
	{
		output.writeToFiles(&mat[i]);
		output.writeToFiles(&node_name[i]);

		if (diffuse_map[i] != nullptr)
		{
			output.writeToFiles(&diffuse_map[i]);
		}
		if (normal_map[i] != nullptr)
		{
			output.writeToFiles(&normal_map[i]);
		}
		if (specular_map[i] != nullptr)
		{
			output.writeToFiles(&specular_map[i]);
		}
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
		output.writeToFiles(cameraParentIDs[i].data(), cameraParentIDs[i].size());
		output.writeToFiles(&camreNames[i]);
	}

	for (unsigned int i = 0; i < lightbody.size(); i++)
	{	
		output.writeToFiles(&lightbody[i]);
		output.writeToFiles(expParentLightID[i].data(), expParentLightID[i].size());
		output.writeToFiles(&expLightName[i]);
	}

	for (unsigned int i = 0; i < nurbBody.size(); i++)
	{
		output.writeToFiles(&nurbBody[i]);
		output.writeToFiles(expParentID[i].data(), expParentID[i].size());
		output.writeToFiles(&expNurbName[i]);
	}

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		output.writeToFiles(&meshes[i]);
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

	for (unsigned int i = 0; i < morphs.size(); i++)
	{
		output.writeToFiles(&morphs[i]);
		output.writeToFiles(morphsPositions[i].data(), morphsPositions[i].size());
		output.writeToFiles(&moprhsNames[i]);
	}

	for (unsigned int i = 0; i < skinStorage.size(); i++)
	{
		output.writeToFiles(&skinStorage[i]);
		output.writeToFiles(influenceIndices[i].data(), influenceIndices[i].size());
		output.writeToFiles(influenceWeights[i].data(), influenceWeights[i].size());
	}

	for (unsigned int i = 0; i < keysStorage.size(); i++)
	{
		output.writeToFiles(&keysStorage[i]);
		output.writeToFiles(points[i].data(), points[i].size());
		output.writeToFiles(&curveName[i]);
		output.writeToFiles(&attachToName[i]);
	}


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