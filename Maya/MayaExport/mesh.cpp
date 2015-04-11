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

	unsigned int numVertices = mesh.numVertices(&status);
	unsigned int numUVs = mesh.numUVs(&status);
	unsigned int numNormals = mesh.numNormals(&status);

	MIntArray triangleCount;
	MIntArray triangleVertices;
	status = mesh.getTriangles(triangleCount, triangleVertices);

	double** verticesPos = new double*[numVertices];
	double** verticesNormal = new double*[numVertices];
	float** verticesUV = new float*[numVertices];

	for (unsigned int i = 0; i < numVertices; i++)
	{
		verticesPos[i] = new double[4];
		verticesNormal[i] = new double[4];
		verticesUV[i] = new float[2];
	}

	//for (unsigned int vertexID = 0; vertexID < numVertices; vertexID++)
	//{
	//	MPoint Pos;
	//	mesh.getPoint(vertexID, Pos, MSpace::kWorld);
	//	Pos.get(verticesPos[vertexID]);

	//	MGlobal::displayInfo(MString() + "ID: " + vertexID);
	//	MGlobal::displayInfo(MString() + "Position: " + Pos.x + " " + Pos.y + " " + Pos.z);
	//}

	MPointArray vertexArray;
	mesh.getPoints(vertexArray, MSpace::kWorld);
	for (unsigned int i = 0; i < numVertices; i++)
	{
		MGlobal::displayInfo(MString() + i + " | Position: " + vertexArray[i].x + " " + vertexArray[i].y + " " + vertexArray[i].z);
	}

	/*for (unsigned int UVcounter = 0; UVcounter < numUVs; UVcounter++)
	{
		float u;
		float v;
		const MString* uvSet = NULL;
		mesh.getUV(UVcounter, u, v, uvSet);
		MGlobal::displayInfo(MString() + UVcounter + " | UV: " + u + " " + v);
	}*/

	MFloatArray uArray;
	MFloatArray vArray;
	mesh.getUVs(uArray, vArray, NULL);
	for (unsigned int i = 0; i < numUVs; i++)
	{
		MGlobal::displayInfo(MString() + i + " | UV: " + uArray[i] + " " + vArray[i]);
	}

	MFloatVectorArray Norm;
	mesh.getNormals(Norm, MSpace::kWorld);
	for (unsigned int i = 0; i < numNormals; i++)
	{
		MGlobal::displayInfo(MString() + i + " | Norm: " + Norm[i].x + " " + Norm[i].y + " " + Norm[i].z);
	}
	
	MFloatVectorArray tangent;
	mesh.getTangents(tangent, MSpace::kWorld, NULL);
	for (unsigned int i = 0; i < tangent.length(); i++)
	{
		MGlobal::displayInfo(MString() + i + " | Tangent: " + tangent[i].x + " " + tangent[i].y + " " + tangent[i].z);
	}

	MFloatVectorArray biNormals;
	mesh.getBinormals(biNormals, MSpace::kWorld, NULL);
	for (unsigned int i = 0; i < biNormals.length(); i++)
	{
		MGlobal::displayInfo(MString() + i + " | Bi-Tangent: " + biNormals[i].x + " " + biNormals[i].y + " " + biNormals[i].z);
	}

	MIntArray triangleCounts;
	MIntArray indicies;
	mesh.getTriangles(triangleCounts, indicies);
	for (unsigned int i = 0; i < indicies.length(); i++)
	{
		MGlobal::displayInfo(MString() + i + " | Indicies: " + indicies[i]);
	}

	MItDependencyNodes it(MFn::kLambert);
	for (; !it.isDone(); it.next())
	{
		MObject mat = it.item();
		MGlobal::displayInfo("hej");
	}
	
	unsigned int instanceNumber;
	MObjectArray shaders;
	MIntArray shaderIndicies;
	//mesh.getConnectedShaders(instanceNumber, shaders, shaderIndicies);
	MGlobal::displayInfo(MString() + shaders.length());

	MGlobal::displayInfo(MString() + "TriangleCount: " + indicies.length());
	MGlobal::displayInfo(MString() + "Verticies: " + numVertices);
	MGlobal::displayInfo(MString() + "UV total: " + numUVs);
	MGlobal::displayInfo(MString() + "Normal total: " + numNormals);

	meshes.meshHeader.vertex_count = numVertices;
	meshes.meshHeader.triangle_count = indicies.length() / 3;
	meshes.vertices.resize(indicies.length());
	
	int NormControl = 0;
	int NormCounter = 0;
	int NormCounter1 = 0;
	int store = 0;
	for (unsigned int i = 0; i < indicies.length(); i++)
	{
		int tmp;
		tmp = indicies[i];
		meshes.vertices[i].position[0] = vertexArray[tmp].x;
		meshes.vertices[i].position[1] = vertexArray[tmp].y;
		meshes.vertices[i].position[2] = vertexArray[tmp].z;
		//MGlobal::displayInfo(MString() + i + " | POSITION: " + meshes.vertices[i].position[0] + " " + meshes.vertices[i].position[1] + " " + meshes.vertices[i].position[2]);

		meshes.vertices[i].uv[0] = uArray[0];
		meshes.vertices[i].uv[1] = vArray[0];

		meshes.vertices[i].normal[0] = Norm[NormCounter].x;
		meshes.vertices[i].normal[1] = Norm[NormCounter].y;
		meshes.vertices[i].normal[2] = Norm[NormCounter].z;
		//MGlobal::displayInfo(MString() + i + " | NORMAL: " + meshes.vertices[i].normal[0] + " " + meshes.vertices[i].normal[1] + " " + meshes.vertices[i].normal[2]);
		
		MGlobal::displayInfo(MString() + "F: " + (tmp+1)/*(indicies[i]+1)*/ + " | " + " | " + (NormCounter+1));	
		
		if (NormControl != 2)
		{
			NormCounter++;
			NormControl++;
			NormCounter1++;
		}
		else
		{
			NormControl = 0;
			NormCounter1++;
		}

		if (NormCounter1 == 4 || NormCounter1 == 5)
		{
			if (NormCounter1 == 4)
			{
				store = NormCounter;
				NormCounter = NormCounter - 2;
			}

			if (NormCounter1 == 5)
			{
				NormCounter = store;
				NormCounter1 = -1;
			}
		}

		if (NormCounter1 == 0)
		{
			NormCounter++;
		}

	}

	/*for (unsigned int i = 0; i < numNormals; i++)
	{
		meshes.vertices[i].normal[0] = Norm[i].x;
		meshes.vertices[i].normal[1] = Norm[i].y;
		meshes.vertices[i].normal[2] = Norm[i].z;
		MGlobal::displayInfo(MString() + i + " | NORMAL: " + meshes.vertices[i].normal[0] + " " + meshes.vertices[i].normal[1] + " " + meshes.vertices[i].normal[2]);
	}*/

	//for (unsigned int i = 0; i < numUVs; i++)
	//{
	//	meshes.vertices[i].uv[0] = uArray[i];
	//	meshes.vertices[i].uv[1] = vArray[i];
	//	MGlobal::displayInfo(MString() + i + " | UV: " + meshes.vertices[i].uv[0] + " " + meshes.vertices[i].uv[1]);
	//}

	return MStatus::kSuccess;
}

MStatus Mesh::exportJoints(MFnMesh& mesh)
{
	return MStatus::kSuccess;
}