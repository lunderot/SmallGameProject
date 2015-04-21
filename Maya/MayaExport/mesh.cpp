#include "mesh.h"
#include < maya/MPlug.h >

MStatus Mesh::exportMesh(MFnMesh& mesh, map<const char*, int>& materials, map<const char*, int> transformHeiraki, meshStruct& meshes, MeshHeader& mayaMeshHeader, map<const char*, unsigned int> meshMap)
{
	MStatus status;
	MString cmdAll = "select -r " + mesh.name();
	MGlobal::executeCommand(cmdAll);
	MString cmdF = "polyEvaluate -f";
	MIntArray faces;
	MGlobal::executeCommand(cmdF, faces, true, false);
	cmdF = "polyEvaluate -t";
	MIntArray triangles;
	MGlobal::executeCommand(cmdF, triangles, true, false);
	if (faces[0] == triangles[0])
	{
		status = exportMaterial(mesh, materials, meshes, mayaMeshHeader);
		status = exportVertices(mesh, transformHeiraki, materials, meshes, mayaMeshHeader);
		status = exportJoints(mesh);
	}
	else
	{
		MGlobal::displayError("TRIANGULATE " + mesh.name());
		return MStatus::kFailure;
	}

	meshMap[mesh.name().asChar()] = meshMap.size();

	return status;
}

MStatus Mesh::exportMaterial(MFnMesh& mesh, map<const char*, int>& materials, meshStruct& meshes, MeshHeader& mayaMeshHeader)
{
	MItDependencyNodes it(MFn::kLambert);
	for (; !it.isDone(); it.next())
	{
		MObject mat = it.item();
		MFnDagNode storeMat(mat);
	}

	MPlugArray material_array;
	meshes.material_Id.resize(mesh.parentCount());
	for (unsigned int i = 0; i < mesh.parentCount(); i++)
	{
		MObjectArray shader_array;
		MIntArray shaderID_array;
		mesh.getConnectedShaders(i, shader_array, shaderID_array);

		if (shaderID_array[0] != -1)
		{

			MObject shading = shader_array[0];
			MFnDependencyNode fn(shading);
			MPlug shader = fn.findPlug("surfaceShader");
			shader.connectedTo(material_array, true, false);

			if (material_array.length())
			{
				unsigned int tmp_materialId;
				MFnDependencyNode fnMat(material_array[0].node());
				tmp_materialId = materials[fnMat.name().asChar()];
				meshes.material_Id[i] = tmp_materialId;

				MGlobal::displayInfo(MString() + "Material ID: " + tmp_materialId + " | name: " + fnMat.name().asChar());
			}
		}
		else
			meshes.material_Id[i] = -1;

		mayaMeshHeader.material_count = mesh.parentCount();
	}

	return MStatus::kSuccess;
}

MStatus Mesh::exportVertices(MFnMesh& mesh, map<const char*, int> transformHeiraki, map<const char*, int>& materials, meshStruct& meshes, MeshHeader& mayaMeshHeader)
{
	MStatus status;



	// POSITION - get information
	MPointArray vertex_array;
	mesh.getPoints(vertex_array, MSpace::kObject);
	meshes.position.resize(vertex_array.length());
	for (unsigned int i = 0; i < vertex_array.length(); i++)
	{
		meshes.position[i].push_back(vertex_array[i].x);
		meshes.position[i].push_back(vertex_array[i].y);
		meshes.position[i].push_back(vertex_array[i].z);
	}
	MGlobal::displayInfo("");

	// UV - get information
	MFloatArray u_array;
	MFloatArray v_array;
	mesh.getUVs(u_array, v_array, NULL);
	meshes.uv.resize(u_array.length());
	for (unsigned int i = 0; i < u_array.length(); i++)
	{
		meshes.uv[i].push_back(u_array[i]);
		meshes.uv[i].push_back(v_array[i]);
	}
	
	// NORMAL - get information
	MFloatVectorArray normal_array_vector;
	mesh.getNormals(normal_array_vector, MSpace::kObject);
	meshes.normal.resize(normal_array_vector.length());
	for (unsigned int i = 0; i < normal_array_vector.length(); i++)
	{
		meshes.normal[i].push_back(normal_array_vector[i].x);
		meshes.normal[i].push_back(normal_array_vector[i].y);
		meshes.normal[i].push_back(normal_array_vector[i].z);
	}

	// TANGENT - get information
	MFloatVectorArray tangent_array_vector;
	mesh.getTangents(tangent_array_vector, MSpace::kObject, NULL);
	meshes.tangent.resize(tangent_array_vector.length());
	for (unsigned int i = 0; i < tangent_array_vector.length(); i++)
	{
		meshes.tangent[i].push_back(tangent_array_vector[i].x);
		meshes.tangent[i].push_back(tangent_array_vector[i].y);
		meshes.tangent[i].push_back(tangent_array_vector[i].z);
	}

	// BI-NORMAL "BI-TANGENT" - fix information
	MFloatVectorArray biNormal_array_vector;
	mesh.getBinormals(biNormal_array_vector, MSpace::kObject, NULL);
	meshes.bi_tangent.resize(biNormal_array_vector.length());
	for (unsigned int i = 0; i < biNormal_array_vector.length(); i++)
	{
		meshes.bi_tangent[i].push_back(biNormal_array_vector[i].x);
		meshes.bi_tangent[i].push_back(biNormal_array_vector[i].y);
		meshes.bi_tangent[i].push_back(biNormal_array_vector[i].z);
	}


	// -------------------------- 
	// INDICIES - get ID
	MIntArray triangleCount_array;
	MIntArray indicie_array;
	mesh.getTriangles(triangleCount_array, indicie_array);

	// UV - get ID
	MIntArray uvId_array;
	for (unsigned int i = 0; i < triangleCount_array.length(); i++)
	{
		for (unsigned int k = 0; k < 3; k++)
		{
			int tmpStore;
			mesh.getPolygonUVid(i, k, tmpStore, NULL);
			uvId_array.append(tmpStore);
		}
	}

	// NORMAL - get ID
	MIntArray normalcount_array;
	MIntArray normalId_array;
	mesh.getNormalIds(normalcount_array, normalId_array);
	


	mayaMeshHeader.name_length = mesh.name().length();
	mayaMeshHeader.vertex_count = vertex_array.length();
	mayaMeshHeader.indice_count = indicie_array.length();
	mayaMeshHeader.position_count = vertex_array.length();
	mayaMeshHeader.uv_count = u_array.length();
	mayaMeshHeader.normal_count = normal_array_vector.length();
	mayaMeshHeader.tangent_count = tangent_array_vector.length();
	mayaMeshHeader.biTangent_count = biNormal_array_vector.length();
	mayaMeshHeader.transform_count = mesh.parentCount();
	
	meshes.name = mesh.name().asChar();
	meshes.vertices.resize(indicie_array.length());
	for (unsigned int i = 0; i < indicie_array.length(); i++)
	{
		meshes.vertices[i].position = indicie_array[i];
		meshes.vertices[i].uv = uvId_array[i];
		meshes.vertices[i].normal = normalId_array[i];
	}
	
	meshes.transform_Id.resize(mayaMeshHeader.transform_count);
	for (unsigned int i = 0; i < mesh.parentCount(); i++)
	{
		MObject parent = mesh.parent(i);
		MFnDagNode storeParent(parent);
		meshes.transform_Id[i] = transformHeiraki[storeParent.name().asChar()];
	}

	MGlobal::displayInfo("STRUCT - MESHSTRUCT");
	MGlobal::displayInfo(MString() + "name: " + meshes.name);
	MGlobal::displayInfo("-----");
	MGlobal::displayInfo("STRUCT - MESHHEADER");
	MGlobal::displayInfo(MString() + "name_length: " + mayaMeshHeader.name_length);
	MGlobal::displayInfo(MString() + "vertex_count: " + mayaMeshHeader.vertex_count);
	MGlobal::displayInfo(MString() + "indice_count: " + mayaMeshHeader.indice_count);

	return MStatus::kSuccess;
}

MStatus Mesh::exportJoints(MFnMesh& mesh)
{
	return MStatus::kSuccess;
}