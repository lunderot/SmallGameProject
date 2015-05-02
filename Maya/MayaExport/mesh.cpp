#include "mesh.h"
#include < maya/MPlug.h >

MStatus Mesh::exportMesh(MFnMesh& mesh, map<const char*, int>& materials, map<const char*, int>& transformHeiraki, meshStruct& meshes, map<const char*, unsigned int>& meshMap,
	vector<vector<double>>& position,
	vector<vector<float>>& uv,
	vector<vector<double>>& normal,
	vector<vector<double>>& tangent,
	vector<vector<double>>& bi_tangent,
	vector<vector<int>>& transform_Id,
	vector<vector<int>>& material_Id,
	vector<vector<Vertex>>& vertices,
	vector<const char*>& name)
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
		status = exportMaterial(mesh, materials, meshes, material_Id);
		status = exportVertices(mesh, transformHeiraki, materials, meshes,
			position, uv, normal, tangent, bi_tangent, transform_Id, vertices, name);
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

MStatus Mesh::exportMaterial(MFnMesh& mesh, map<const char*, int>& materials, meshStruct& meshes,
	vector<vector<int>>& material_Id)
{
	MItDependencyNodes it(MFn::kLambert);
	for (; !it.isDone(); it.next())
	{
		MObject mat = it.item();
		MFnDagNode storeMat(mat);
	}

	MPlugArray material_array;
	vector<int> vector_Material_Id(mesh.parentCount());
	//meshes.material_Id = new int[mesh.parentCount()];
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
				vector_Material_Id[i] = tmp_materialId;
				//meshes.material_Id[i] = tmp_materialId;

				MGlobal::displayInfo(MString() + "Material ID: " + tmp_materialId + " | name: " + fnMat.name().asChar());
			}
		}
		else
			vector_Material_Id[i] = -1;
			//meshes.material_Id[i] = -1;

		meshes.material_count = mesh.parentCount();
	}
	material_Id.push_back(vector_Material_Id);

	return MStatus::kSuccess;
}

MStatus Mesh::exportVertices(MFnMesh& mesh, map<const char*, int>& transformHeiraki, map<const char*, int>& materials, meshStruct& meshes,
	vector<vector<double>>& position,
	vector<vector<float>>& uv,
	vector<vector<double>>& normal,
	vector<vector<double>>& tangent,
	vector<vector<double>>& bi_tangent,
	vector<vector<int>>& transform_Id,
	vector<vector<Vertex>>& vertices,
	vector<const char*>& name)
{
	MStatus status;



	// POSITION - get information
	MPointArray vertex_array;
	mesh.getPoints(vertex_array, MSpace::kObject);
	//position.resize(vertex_array.length());
	//meshes.position = new double*[vertex_array.length()];
	vector<double> vertexPos;
	for (unsigned int i = 0; i < vertex_array.length(); i++)
	{
		vertexPos.push_back(vertex_array[i].x);
		vertexPos.push_back(vertex_array[i].y);
		vertexPos.push_back(vertex_array[i].z);
		//position[i][0] = vertex_array[i].x;
		//meshes.position[i] = new double[3];
		//meshes.position[i][0] = vertex_array[i].x;
		//meshes.position[i][1] = vertex_array[i].y;
		//meshes.position[i][2] = vertex_array[i].z;
	}
	position.push_back(vertexPos);
	MGlobal::displayInfo("");

	// UV - get information
	MFloatArray u_array;
	MFloatArray v_array;
	mesh.getUVs(u_array, v_array, NULL);
	//meshes.uv = new float*[u_array.length()];
	vector<float> uvPos;
	for (unsigned int i = 0; i < u_array.length(); i++)
	{
		uvPos.push_back(u_array[i]);
		uvPos.push_back(v_array[i]);
		//meshes.uv[i] = new float[2];
		//meshes.uv[i][0] = u_array[i];
		//meshes.uv[i][1] = v_array[i];
	}
	uv.push_back(uvPos);

	// NORMAL - get information
	MFloatVectorArray normal_array_vector;
	mesh.getNormals(normal_array_vector, MSpace::kObject);
	//meshes.normal = new double*[normal_array_vector.length()];
	vector<double> normalPos;
	for (unsigned int i = 0; i < normal_array_vector.length(); i++)
	{
		normalPos.push_back(normal_array_vector[i].x);
		normalPos.push_back(normal_array_vector[i].y);
		normalPos.push_back(normal_array_vector[i].z);
		//meshes.normal[i] = new double[3];
		//meshes.normal[i][0] = normal_array_vector[i].x;
		//meshes.normal[i][1] = normal_array_vector[i].y;
		//meshes.normal[i][2] = normal_array_vector[i].z;
	}
	normal.push_back(normalPos);

	// TANGENT - get information
	MFloatVectorArray tangent_array_vector;
	mesh.getTangents(tangent_array_vector, MSpace::kObject, NULL);
	//meshes.tangent = new double*[tangent_array_vector.length()];
	vector<double> tangentPos;
	for (unsigned int i = 0; i < tangent_array_vector.length(); i++)
	{
		tangentPos.push_back(tangent_array_vector[i].x);
		tangentPos.push_back(tangent_array_vector[i].y);
		tangentPos.push_back(tangent_array_vector[i].z);
		//meshes.tangent[i] = new double[3];
		//meshes.tangent[i][0] = tangent_array_vector[i].x;
		//meshes.tangent[i][1] = tangent_array_vector[i].y;
		//meshes.tangent[i][2] = tangent_array_vector[i].z;
	}
	tangent.push_back(tangentPos);

	// BI-NORMAL "BI-TANGENT" - fix information
	MFloatVectorArray biNormal_array_vector;
	mesh.getBinormals(biNormal_array_vector, MSpace::kObject, NULL);
	//meshes.bi_tangent = new double*[biNormal_array_vector.length()];
	vector<double> biTangentPos;
	for (unsigned int i = 0; i < biNormal_array_vector.length(); i++)
	{
		biTangentPos.push_back(biNormal_array_vector[i].x);
		biTangentPos.push_back(biNormal_array_vector[i].y);
		biTangentPos.push_back(biNormal_array_vector[i].z);
		//meshes.bi_tangent[i] = new double[3];
		//meshes.bi_tangent[i][0] = biNormal_array_vector[i].x;
		//meshes.bi_tangent[i][1] = biNormal_array_vector[i].y;
		//meshes.bi_tangent[i][2] = biNormal_array_vector[i].z;
	}
	bi_tangent.push_back(biTangentPos);

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

	name.push_back(mesh.name().asChar());
	vector<Vertex> vector_tmp_vertices;
	//vector<int> test_indicie;
	//vector<int> test_uvId;
	//vector<int> test_normalId;
	vector_tmp_vertices.resize(indicie_array.length());
	//meshes.name = mesh.name().asChar();
	//meshes.vertices = new Vertex[indicie_array.length()];
	for (unsigned int i = 0; i < indicie_array.length(); i++)
	{
		//test_indicie.push_back(indicie_array[i]);
		//test_uvId.push_back(uvId_array[i]);
		//test_normalId.push_back(normalId_array[i]);
		vector_tmp_vertices[i].position = indicie_array[i];
		vector_tmp_vertices[i].uv = uvId_array[i];
		vector_tmp_vertices[i].normal = normalId_array[i];
		//test[i].normal = indicie_array[i];
		//test[i].position = uvId_array[i];
		//test[i].uv = normalId_array[i];
		//meshes.vertices[i].position = indicie_array[i];
		//meshes.vertices[i].uv = uvId_array[i];
		//meshes.vertices[i].normal = normalId_array[i];
	}
	vertices.push_back(vector_tmp_vertices);

	vector<int> vector_Transform_Id(mesh.parentCount());
	//meshes.transform_Id = new int[meshes.transform_count];
	for (unsigned int i = 0; i < mesh.parentCount(); i++)
	{
		MObject parent = mesh.parent(i);
		MFnDagNode storeParent(parent);
		vector_Transform_Id[i] = transformHeiraki[storeParent.name().asChar()];
		//meshes.transform_Id[i] = transformHeiraki[storeParent.name().asChar()];
	}
	transform_Id.push_back(vector_Transform_Id);

	//MGlobal::displayInfo("STRUCT - MESHSTRUCT");
	//MGlobal::displayInfo(MString() + "name: " + mesh.name().asChar());
	//MGlobal::displayInfo("-----");
	//MGlobal::displayInfo("STRUCT - MESHHEADER");
	//MGlobal::displayInfo(MString() + "name_length: " + meshes.name_length);
	//MGlobal::displayInfo(MString() + "vertex_count: " + meshes.vertex_count);
	//MGlobal::displayInfo(MString() + "indice_count: " + meshes.indice_count);

	return MStatus::kSuccess;
}

MStatus Mesh::exportJoints(MFnMesh& mesh)
{
	return MStatus::kSuccess;
}