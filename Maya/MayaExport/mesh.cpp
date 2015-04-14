#include "mesh.h"
#include < maya/MPlug.h >

MStatus Mesh::exportMesh(MFnMesh& mesh, map<const char*, unsigned int>& materials, map<const char*, int> transformHeiraki, meshStruct& meshes, MeshHeader& mayaMeshHeader)
{
	MStatus status;
	status = exportMaterial(mesh, materials);
	status = exportVertices(mesh, transformHeiraki, materials, meshes, mayaMeshHeader);
	status = exportJoints(mesh);

	return status;
}

MStatus Mesh::exportMaterial(MFnMesh& mesh, map<const char*, unsigned int>& materials)
{
	return MStatus::kSuccess;
}

MStatus Mesh::exportVertices(MFnMesh& mesh, map<const char*, int> transformHeiraki, map<const char*, unsigned int>& transform_materials, meshStruct& meshes, MeshHeader& mayaMeshHeader)
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
		MGlobal::displayInfo(MString() + i + " | Position: " + vertex_array[i].x + " " + vertex_array[i].y + " " + vertex_array[i].z);
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
		MGlobal::displayInfo(MString() + i + " | UV: " + u_array[i] + " " + v_array[i]);
	}
	MGlobal::displayInfo("");
	
	// NORMAL - get information
	MFloatVectorArray normal_array_vector;
	mesh.getNormals(normal_array_vector, MSpace::kObject);
	meshes.normal.resize(normal_array_vector.length());
	for (unsigned int i = 0; i < normal_array_vector.length(); i++)
	{
		meshes.normal[i].push_back(normal_array_vector[i].x);
		meshes.normal[i].push_back(normal_array_vector[i].y);
		meshes.normal[i].push_back(normal_array_vector[i].z);
		MGlobal::displayInfo(MString() + i + " | Normal: " + normal_array_vector[i].x + " " + normal_array_vector[i].y + " " + normal_array_vector[i].z);
	}
	MGlobal::displayInfo("");

	// TANGENT - get information
	MFloatVectorArray tangent_array_vector;
	mesh.getTangents(tangent_array_vector, MSpace::kObject, NULL);
	meshes.tangent.resize(tangent_array_vector.length());
	for (unsigned int i = 0; i < tangent_array_vector.length(); i++)
	{
		meshes.tangent[i].push_back(tangent_array_vector[i].x);
		meshes.tangent[i].push_back(tangent_array_vector[i].y);
		meshes.tangent[i].push_back(tangent_array_vector[i].z);
		MGlobal::displayInfo(MString() + i + " | Tangent: " + tangent_array_vector[i].x + " " + tangent_array_vector[i].y + " " + tangent_array_vector[i].z);
	}
	MGlobal::displayInfo("");

	// BI-NORMAL "BI-TANGENT" - fix information
	MFloatVectorArray biNormal_array_vector;
	mesh.getBinormals(biNormal_array_vector, MSpace::kObject, NULL);
	meshes.bi_tangent.resize(biNormal_array_vector.length());
	for (unsigned int i = 0; i < biNormal_array_vector.length(); i++)
	{
		meshes.bi_tangent[i].push_back(biNormal_array_vector[i].x);
		meshes.bi_tangent[i].push_back(biNormal_array_vector[i].y);
		meshes.bi_tangent[i].push_back(biNormal_array_vector[i].z);
		MGlobal::displayInfo(MString() + i + " | Bi-Tangent: " + biNormal_array_vector[i].x + " " + biNormal_array_vector[i].y + " " + biNormal_array_vector[i].z);
	}
	MGlobal::displayInfo("");


	// -------------------------- 
	// INDICIES - get ID
	MIntArray triangleCount_array;
	MIntArray indicie_array;
	mesh.getTriangles(triangleCount_array, indicie_array);
	//meshes.indices.resize(indicie_array.length());
	for (unsigned int i = 0; i < indicie_array.length(); i++)
	{
		//meshes.indices[i] = indicie_array[i];
		MGlobal::displayInfo(MString() + i + " | Indicies: " + indicie_array[i]);
	}
	MGlobal::displayInfo(MString() + indicie_array.length());

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
	MGlobal::displayInfo(MString() + "UV total: " + u_array.length()); 
	MGlobal::displayInfo(MString() + "Normal total: " + normal_array_vector.length());
	MGlobal::displayInfo("");


	meshes.name = mesh.name().asChar();
	//meshes.meshHeader.transform_count = mesh.parentCount();
	mayaMeshHeader.transform_count = mesh.parentCount();
	meshes.transform_Id.resize(mayaMeshHeader.transform_count);
	for (unsigned int i = 0; i < mesh.parentCount(); i++)
	{
		MObject parent = mesh.parent(i);
		MFnDagNode storeParent(parent);
		meshes.transform_Id[i] = transformHeiraki[storeParent.name().asChar()];
		MGlobal::displayInfo(MString() + storeParent.name().asChar());
	}

	meshes.vertices_count = vertex_array.length();
	meshes.vertices.resize(indicie_array.length());
	//meshes.indices.resize(indicie_array.length());

	mayaMeshHeader.name_length = mesh.name().length();
	mayaMeshHeader.triangle_count = indicie_array.length();

	mayaMeshHeader.vertex_count = vertex_array.length();

	//meshes.meshHeader.name_length = mesh.name().length();
	//meshes.meshHeader.vertex_count = vertex_array.length();
	//meshes.meshHeader.triangle_count = triangleCount_array.length();
	MGlobal::displayInfo("STRUCT - MESHSTRUCT");
	MGlobal::displayInfo(MString() + "name: " + meshes.name);
	MGlobal::displayInfo("-----");
	MGlobal::displayInfo("STRUCT - MESHHEADER");
	MGlobal::displayInfo(MString() + "name_length: " + mayaMeshHeader.name_length);
	MGlobal::displayInfo(MString() + "vertex_count: " + mayaMeshHeader.vertex_count);
	MGlobal::displayInfo(MString() + "triangle_count: " + mayaMeshHeader.triangle_count);
	MGlobal::displayInfo("");
	

	/*for (unsigned int i = 0; i < indicie_array.length(); i++)
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
	}*/

	for (unsigned int i = 0; i < indicie_array.length(); i++)
	{
		meshes.vertices[i].position = indicie_array[i];
		meshes.vertices[i].uv = uvId_array[i];
		meshes.vertices[i].normal = normalId_array[i];
	}

	MItDependencyNodes it(MFn::kLambert);
	for (; !it.isDone(); it.next())
	{
		MObject mat = it.item();
		MFnDagNode storeMat(mat);
	}
	MGlobal::displayInfo("");

	MPlugArray materials;
	meshes.material_Id.resize(mesh.parentCount());
	for (unsigned int i = 0; i < mesh.parentCount(); i++)
	{
		MObjectArray shaderss;
		MIntArray shadersindi;
		mesh.getConnectedShaders(i, shaderss, shadersindi);

		MObject shading = shaderss[0];
		MFnDependencyNode fn(shading);
		MPlug sshader = fn.findPlug("surfaceShader");

		sshader.connectedTo(materials, true, false);
		MGlobal::displayInfo(MString() + "i: " + i);

		if (materials.length())
		{
			unsigned int test;
			MFnDependencyNode fnMat(materials[0].node());
			test = transform_materials[fnMat.name().asChar()]; 
			meshes.material_Id[i] = test;
			
			MGlobal::displayInfo(MString() + test);
			MGlobal::displayInfo(MString() + "Material name: " + fnMat.name().asChar());
		}
		mayaMeshHeader.material_count = mesh.parentCount();
		//meshes.meshHeader.material_count = mesh.parentCount();
	}
	MGlobal::displayInfo(MString() + materials.length());

	unsigned int instanceNumber;
	MObjectArray shaders;
	MIntArray shaderIndicies;
	mesh.getConnectedShaders(2, shaders, shaderIndicies);
	MGlobal::displayInfo(MString() + shaders.length());
	MGlobal::displayInfo(MString() + shaderIndicies.length());

	return MStatus::kSuccess;

}

MStatus Mesh::exportJoints(MFnMesh& mesh)
{
	return MStatus::kSuccess;
}