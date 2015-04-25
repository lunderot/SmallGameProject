#include "mesh.h"
#include < maya/MPlug.h >

MStatus Mesh::exportMesh(MFnMesh& mesh, map<const char*, int>& materials, map<const char*, int>& transformHeiraki, meshStruct& meshes, map<const char*, unsigned int>& meshMap)
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
		status = exportMaterial(mesh, materials, meshes);
		status = exportVertices(mesh, transformHeiraki, materials, meshes);
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

MStatus Mesh::exportMaterial(MFnMesh& mesh, map<const char*, int>& materials, meshStruct& meshes)
{
	MItDependencyNodes it(MFn::kLambert);
	for (; !it.isDone(); it.next())
	{
		MObject mat = it.item();
		MFnDagNode storeMat(mat);
	}

	MPlugArray material_array;
	meshes.material_Id = new int[mesh.parentCount()];
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

		meshes.material_count = mesh.parentCount();
	}

	return MStatus::kSuccess;
}

MStatus Mesh::exportVertices(MFnMesh& mesh, map<const char*, int>& transformHeiraki, map<const char*, int>& materials, meshStruct& meshes)
{
	MStatus status;



	// POSITION - get information
	MPointArray vertex_array;
	mesh.getPoints(vertex_array, MSpace::kObject);
	meshes.position = new double*[vertex_array.length()];
	for (unsigned int i = 0; i < vertex_array.length(); i++)
	{
		meshes.position[i] = new double[3];
		meshes.position[i][0] = vertex_array[i].x;
		meshes.position[i][1] = vertex_array[i].y;
		meshes.position[i][2] = vertex_array[i].z;
	}
	MGlobal::displayInfo("");

	// UV - get information
	MFloatArray u_array;
	MFloatArray v_array;
	mesh.getUVs(u_array, v_array, NULL);
	meshes.uv = new float*[u_array.length()];
	for (unsigned int i = 0; i < u_array.length(); i++)
	{
		meshes.uv[i] = new float[2];
		meshes.uv[i][0] = u_array[i];
		meshes.uv[i][1] = v_array[i];
	}

	// NORMAL - get information
	MFloatVectorArray normal_array_vector;
	mesh.getNormals(normal_array_vector, MSpace::kObject);
	meshes.normal = new double*[normal_array_vector.length()];
	for (unsigned int i = 0; i < normal_array_vector.length(); i++)
	{
		meshes.normal[i] = new double[3];
		meshes.normal[i][0] = normal_array_vector[i].x;
		meshes.normal[i][1] = normal_array_vector[i].y;
		meshes.normal[i][2] = normal_array_vector[i].z;
	}

	// TANGENT - get information
	MFloatVectorArray tangent_array_vector;
	mesh.getTangents(tangent_array_vector, MSpace::kObject, NULL);
	meshes.tangent = new double*[tangent_array_vector.length()];
	for (unsigned int i = 0; i < tangent_array_vector.length(); i++)
	{
		meshes.tangent[i] = new double[3];
		meshes.tangent[i][0] = tangent_array_vector[i].x;
		meshes.tangent[i][1] = tangent_array_vector[i].y;
		meshes.tangent[i][2] = tangent_array_vector[i].z;
	}

	// BI-NORMAL "BI-TANGENT" - fix information
	MFloatVectorArray biNormal_array_vector;
	mesh.getBinormals(biNormal_array_vector, MSpace::kObject, NULL);
	meshes.bi_tangent = new double*[biNormal_array_vector.length()];
	for (unsigned int i = 0; i < biNormal_array_vector.length(); i++)
	{
		meshes.bi_tangent[i] = new double[3];
		meshes.bi_tangent[i][0] = biNormal_array_vector[i].x;
		meshes.bi_tangent[i][1] = biNormal_array_vector[i].y;
		meshes.bi_tangent[i][2] = biNormal_array_vector[i].z;
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

	meshes.name_length = mesh.name().length();
	meshes.vertex_count = vertex_array.length();
	meshes.indice_count = indicie_array.length();
	meshes.position_count = vertex_array.length();
	meshes.uv_count = u_array.length();
	meshes.normal_count = normal_array_vector.length();
	meshes.tangent_count = tangent_array_vector.length();
	meshes.biTangent_count = biNormal_array_vector.length();
	meshes.transform_count = mesh.parentCount();
	

	/*mayaMeshHeader.name_length = mesh.name().length();
	mayaMeshHeader.vertex_count = vertex_array.length();
	mayaMeshHeader.indice_count = indicie_array.length();
	mayaMeshHeader.position_count = vertex_array.length();
	mayaMeshHeader.uv_count = u_array.length();
	mayaMeshHeader.normal_count = normal_array_vector.length();
	mayaMeshHeader.tangent_count = tangent_array_vector.length();
	mayaMeshHeader.biTangent_count = biNormal_array_vector.length();
	mayaMeshHeader.transform_count = mesh.parentCount();*/

	meshes.name = mesh.name().asChar();
	meshes.vertices = new Vertex[indicie_array.length()];
	for (unsigned int i = 0; i < indicie_array.length(); i++)
	{
		meshes.vertices[i].position = indicie_array[i];
		meshes.vertices[i].uv = uvId_array[i];
		meshes.vertices[i].normal = normalId_array[i];
	}

	meshes.transform_Id = new int[meshes.transform_count];
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
	MGlobal::displayInfo(MString() + "name_length: " + meshes.name_length);
	MGlobal::displayInfo(MString() + "vertex_count: " + meshes.vertex_count);
	MGlobal::displayInfo(MString() + "indice_count: " + meshes.indice_count);

	return MStatus::kSuccess;
}

MStatus Mesh::exportJoints(MFnMesh& mesh)
{
	return MStatus::kSuccess;
}