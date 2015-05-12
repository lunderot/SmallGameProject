#include "Mesh.h"
FbxMesh* Mesh::GetMesh(FbxScene* scene, std::string fileName)
{
	// From the Importer
	Importer importer;
	if (!importer.importFile(fileName))
	{
		FBXSDK_printf("Failed to Import the file");
		return NULL;
	}

	// MeshNode
	FbxNode* meshNode = FbxNode::Create(scene, "meshNode");

	// Mesh
	FbxMesh* mesh = FbxMesh::Create(scene, "mesh");

	meshNode->SetNodeAttribute(mesh);

	FbxNode* rootNode = scene->GetRootNode();
	rootNode->AddChild(meshNode);

	const ImporterMesh* importedMeshes = importer.getMesh();
	std::vector <FbxVector4> vertices;

	for (unsigned int i = 0; i < importer.getNumMeshes(); i++)
	{
		for (unsigned int j = 0; j < importedMeshes[i].indice_count; j++)
		{
			FbxVector4 vertex(importedMeshes[i].position[importedMeshes[i].vertices[j].position * 3 + 0], importedMeshes[i].position[importedMeshes[i].vertices[j].position * 3 + 1], importedMeshes[i].position[importedMeshes[i].vertices[j].position * 3 + 2]);
			std::cout << (vertex.mData[0]) << " " << (vertex.mData[1]) << " " << (vertex.mData[2]) << " " << std::endl;
			vertices.push_back(vertex);

		}
	}

	// Define Faces
	mesh->InitControlPoints(6);
	FbxVector4* controlPoints = mesh->GetControlPoints();

	for (unsigned int i = 0; i < 6; i++)
	{
		controlPoints[i] = vertices[i];
		cout << "CONTROL POINT!" << endl;
	}

	// Normals
	FbxVector4 lNormalXPos(1, 0, 0);
	FbxVector4 lNormalXNeg(-1, 0, 0);
	FbxVector4 lNormalYPos(0, 1, 0);
	FbxVector4 lNormalYNeg(0, -1, 0);
	FbxVector4 lNormalZPos(0, 0, 1);
	FbxVector4 lNormalZNeg(0, 0, -1);

	FbxLayer* layer = mesh->GetLayer(0);
	if (layer == NULL)
	{
		mesh->CreateLayer();
		layer = mesh->GetLayer(0);
	}

	FbxLayerElementNormal* layerElementNormal = FbxLayerElementNormal::Create(mesh, "");
	layerElementNormal->SetMappingMode(FbxLayerElement::eByControlPoint);

	layerElementNormal->SetReferenceMode(FbxLayerElement::eDirect);

	for (unsigned int i = 0; i < 6; i++)
	{
		layerElementNormal->GetDirectArray().Add(lNormalXPos);
	}
	layer->SetNormals(layerElementNormal);
}

Mesh::~Mesh()
{

}