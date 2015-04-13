#include "mesh.h"

MStatus Mesh::exportMesh(MFnMesh& mesh, map<const char*, unsigned int>& materials, meshStruct& meshes)
{
	MStatus status;
	status = exportMaterial(mesh, materials);
	status = exportVertices(mesh, meshes);
	status = exportJoints(mesh);

	return status;
}

MStatus Mesh::exportMaterial(MFnMesh& mesh, map<const char*, unsigned int>& materials)
{
	return MStatus::kSuccess;
}

MStatus Mesh::exportVertices(MFnMesh& mesh, meshStruct& meshes)
{
	MStatus status;

	/*unsigned int numVertices = mesh.numVertices(&status);
	unsigned int numUVs = mesh.numUVs(&status);
	unsigned int numNormals = mesh.numNormals(&status);*/

	/*double** verticesPos = new double*[numVertices];
	double** verticesNormal = new double*[numVertices];
	float** verticesUV = new float*[numVertices];

	for (unsigned int i = 0; i < numVertices; i++)
	{
		verticesPos[i] = new double[4];
		verticesNormal[i] = new double[4];
		verticesUV[i] = new float[2];
	}*/

	//MIntArray triangleCount;
	//MIntArray triangleVertices;
	//status = mesh.getTriangles(triangleCount, triangleVertices);

	// POSITION - get information
	MPointArray vertex_array;
	mesh.getPoints(vertex_array, MSpace::kWorld);
	for (unsigned int i = 0; i < vertex_array.length()/*numVertices*/; i++)
	{
		MGlobal::displayInfo(MString() + i + " | Position: " + vertex_array[i].x + " " + vertex_array[i].y + " " + vertex_array[i].z);
	}
	MGlobal::displayInfo("");

	// UV - get information
	MFloatArray u_array;
	MFloatArray v_array;
	mesh.getUVs(u_array, v_array, NULL);
	for (unsigned int i = 0; i < u_array.length()/*numUVs*/; i++)
	{
		MGlobal::displayInfo(MString() + i + " | UV: " + u_array[i] + " " + v_array[i]);
	}
	MGlobal::displayInfo("");

	// NORMAL - get information
	MFloatVectorArray normal_array_vector;
	mesh.getNormals(normal_array_vector, MSpace::kWorld);
	for (unsigned int i = 0; i < normal_array_vector.length()/*numNormals*/; i++)
	{
		MGlobal::displayInfo(MString() + i + " | Normal: " + normal_array_vector[i].x + " " + normal_array_vector[i].y + " " + normal_array_vector[i].z);
	}
	MGlobal::displayInfo("");

	// TANGENT - get information
	MFloatVectorArray tangent_array_vector;
	mesh.getTangents(tangent_array_vector, MSpace::kWorld, NULL);
	for (unsigned int i = 0; i < tangent_array_vector.length(); i++)
	{
		MGlobal::displayInfo(MString() + i + " | Tangent: " + tangent_array_vector[i].x + " " + tangent_array_vector[i].y + " " + tangent_array_vector[i].z);
	}
	MGlobal::displayInfo("");

	// BI-NORMAL "BI-TANGENT" - fix information
	MFloatVectorArray biNormal_array_vector;
	mesh.getBinormals(biNormal_array_vector, MSpace::kWorld, NULL);
	for (unsigned int i = 0; i < biNormal_array_vector.length(); i++)
	{
		MGlobal::displayInfo(MString() + i + " | Bi-Tangent: " + biNormal_array_vector[i].x + " " + biNormal_array_vector[i].y + " " + biNormal_array_vector[i].z);
	}
	MGlobal::displayInfo("");
	
	// MATERIAL - ..............
	MItDependencyNodes it(MFn::kLambert);
	for (; !it.isDone(); it.next())
	{
		MObject mat = it.item();
		MGlobal::displayInfo("hej");
	}
	MGlobal::displayInfo("");

	unsigned int instanceNumber;
	MObjectArray shaders;
	MIntArray shaderIndicies;
	//mesh.getConnectedShaders(instanceNumber, shaders, shaderIndicies);
	//Global::displayInfo(MString() + shaders.length());


	// -------------------------- 
	// INDICIES - get ID
	MIntArray triangleCount_array;
	MIntArray indicie_array;
	mesh.getTriangles(triangleCount_array, indicie_array);
	for (unsigned int i = 0; i < indicie_array.length(); i++)
	{
		MGlobal::displayInfo(MString() + i + " | Indicies: " + indicie_array[i]);
	}
	MGlobal::displayInfo("");

	// UV - get ID
	MIntArray uvId_array;
	for (unsigned int i = 0; i < triangleCount_array.length(); i++)
	{
		MGlobal::displayInfo(MString() + "FACE: " + i);
		for (unsigned int k = 0; k < 3; k++)
		{
			int tmpStore;
			mesh.getPolygonUVid(i, k, tmpStore, NULL);
			uvId_array.append(tmpStore);
			MGlobal::displayInfo(MString() + "tmpStoreUV_ID: " + tmpStore);
		}
		MGlobal::displayInfo("");
	}
	MGlobal::displayInfo(MString() + "uvId_array | LENGTH: " + uvId_array.length());
	MGlobal::displayInfo("");

	// NORMAL - get ID
	MIntArray normalcount_array;
	MIntArray normalId_array;
	mesh.getNormalIds(normalcount_array, normalId_array);
	for (unsigned int i = 0; i < normalId_array.length(); i++)
	{
		MGlobal::displayInfo(MString() + i + " : normal ID: " + normalId_array[i]);
	}
	MGlobal::displayInfo("");

	MGlobal::displayInfo(MString() + normalcount_array.length() + " " + normalId_array.length());
	MGlobal::displayInfo(MString() + "TriangleCount: " + triangleCount_array.length());
	MGlobal::displayInfo(MString() + "Verticies: " + vertex_array.length());
	MGlobal::displayInfo(MString() + "UV total: " + u_array.length()); //elr v_array.length()
	MGlobal::displayInfo(MString() + "Normal total: " + normal_array_vector.length());
	MGlobal::displayInfo("");

	meshes.name = mesh.name().asChar();
	meshes.vertices.resize(indicie_array.length());
	meshes.indices.resize(indicie_array.length());

	meshes.meshHeader.name_length = mesh.name().length();
	meshes.meshHeader.vertex_count = vertex_array.length();
	meshes.meshHeader.triangle_count = triangleCount_array.length();
	MGlobal::displayInfo("STRUCT - MESHSTRUCT");
	MGlobal::displayInfo(MString() + "name: " + meshes.name);
	MGlobal::displayInfo("-----");
	MGlobal::displayInfo("STRUCT - MESHHEADER");
	MGlobal::displayInfo(MString() + "name_length: " + meshes.meshHeader.name_length);
	MGlobal::displayInfo(MString() + "vertex_count: " + meshes.meshHeader.vertex_count);
	MGlobal::displayInfo(MString() + "triangle_count: " + meshes.meshHeader.triangle_count);
	MGlobal::displayInfo("");

	for (unsigned int i = 0; i < indicie_array.length(); i++)
	{
		int tmpPosition;
		tmpPosition = indicie_array[i];
		meshes.vertices[i].position[0] = vertex_array[tmpPosition].x;
		meshes.vertices[i].position[1] = vertex_array[tmpPosition].y;
		meshes.vertices[i].position[2] = vertex_array[tmpPosition].z;

		int tmpUv;
		tmpUv = uvId_array[i];
		meshes.vertices[i].uv[0] = u_array[tmpUv];
		meshes.vertices[i].uv[1] = v_array[tmpUv];

		int tmpNormal;
		tmpNormal = normalId_array[i];
		meshes.vertices[i].normal[0] = normal_array_vector[tmpNormal].x;
		meshes.vertices[i].normal[1] = normal_array_vector[tmpNormal].y;
		meshes.vertices[i].normal[2] = normal_array_vector[tmpNormal].z;

		meshes.vertices[i].tangent[0] = tangent_array_vector[i].x;
		meshes.vertices[i].tangent[1] = tangent_array_vector[i].y;
		meshes.vertices[i].tangent[2] = tangent_array_vector[i].z;

		meshes.vertices[i].bi_tangent[0] = biNormal_array_vector[i].x;
		meshes.vertices[i].bi_tangent[1] = biNormal_array_vector[i].y;
		meshes.vertices[i].bi_tangent[2] = biNormal_array_vector[i].z;
	}

	bool displayControl = true;
	int displayCounter = 0;
	for (unsigned int i = 0; i < indicie_array.length(); i++)
	{
		if (displayControl == true)
		{
			MGlobal::displayInfo(MString() + "F: " + displayCounter + "                    ");
			displayControl = false;
			displayCounter++;
		}
		MGlobal::displayInfo(MString() + "---VERTEX: " + indicie_array[i] + " | " + uvId_array[i] + " | " + normalId_array[i] + " | ( " + i + " | " + i + " ) ---");
		MGlobal::displayInfo(MString() + "POS: " + meshes.vertices[i].position[0] + " " + meshes.vertices[i].position[1] + " " + meshes.vertices[i].position[2]);
		MGlobal::displayInfo(MString() + "UV: " + meshes.vertices[i].uv[0] + " " + meshes.vertices[i].uv[1]);
		MGlobal::displayInfo(MString() + "NORM:       " + meshes.vertices[i].normal[0] + " " + meshes.vertices[i].normal[1] + " " + meshes.vertices[i].normal[2]);
		MGlobal::displayInfo(MString() + "TANGENT:    " + meshes.vertices[i].tangent[0] + " " + meshes.vertices[i].tangent[1] + " " + meshes.vertices[i].tangent[2]);
		MGlobal::displayInfo(MString() + "BI-TANGENT: " + meshes.vertices[i].bi_tangent[0] + " " + meshes.vertices[i].bi_tangent[1] + " " + meshes.vertices[i].bi_tangent[2]);
	}
	return MStatus::kSuccess;
}

MStatus Mesh::exportJoints(MFnMesh& mesh)
{
	return MStatus::kSuccess;
}