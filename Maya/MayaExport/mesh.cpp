#include "mesh.h"
#include "CommonDeclaration.h"

MStatus Mesh::exportMesh(MFnMesh& mesh, map<string, unsigned int>& materials, ofstream& outputFile)
{
	MStatus status;
	status = exportMaterial(mesh, materials, outputFile);
	status = exportVertices(mesh, outputFile);
	status = exportJoints(mesh, outputFile);

	return status;
}

MStatus Mesh::exportMaterial(MFnMesh& mesh, map<string, unsigned int>& materials, ofstream& outputFile)
{
	return MStatus::kSuccess;
}

MStatus Mesh::exportVertices(MFnMesh& mesh, ofstream& outputFile)
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

	for (unsigned int vertexID = 0; vertexID < numVertices; vertexID++)
	{
		MPoint Pos;
		MVector Normal;
		float2 UV;
		mesh.getPoint(vertexID, Pos);
		mesh.getVertexNormal(vertexID, Normal);
		mesh.getUVAtPoint(Pos, UV);
		Pos.get(verticesPos[vertexID]);
		Normal.get(verticesNormal[vertexID]);
		verticesUV[vertexID] = UV;
	}

	for (unsigned int vertexID = 0; vertexID < numVertices; vertexID++)
	{
		outputFile.write((const char*)verticesPos[vertexID], sizeof(double) * 4);
		outputFile.write((const char*)verticesNormal[vertexID], sizeof(double) * 4);
		outputFile.write((const char*)verticesUV[vertexID], sizeof(float) * 2);
	}


	return MStatus::kSuccess;
}

MStatus Mesh::exportJoints(MFnMesh& mesh, ofstream& outputFile)
{
	return MStatus::kSuccess;
}