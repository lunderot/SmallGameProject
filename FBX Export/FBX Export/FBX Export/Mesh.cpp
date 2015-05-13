#include "Mesh.h"

double* QuatToEuler(const double quat[4])
{
	double returnValues[3];

	double sqw = quat[3] * quat[3];
	double sqx = quat[0] * quat[0];
	double sqy = quat[1] * quat[1];
	double sqz = quat[2] * quat[2];
	double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	double test = quat[0] * quat[1] + quat[2] * quat[3];
	if (test > 0.499*unit) { // singularity at north pole
		returnValues[0] = 2 * atan2(quat[0], quat[3]);
		returnValues[1] = 3.1415 / 2;
		returnValues[2] = 0;
		return returnValues;
	}
	if (test < -0.499*unit) { // singularity at south pole
		returnValues[2] = -2 * atan2(quat[0], quat[3]);
		returnValues[1] = -3.1415 / 2;
		returnValues[0] = 0;
		return returnValues;
	}
	returnValues[2] = atan2(2 * quat[1] * quat[3] - 2 * quat[0] * quat[2], sqx - sqy - sqz + sqw);
	returnValues[1] = asin(2 * test / unit);
	returnValues[0] = atan2(2 * quat[0] * quat[3] - 2 * quat[1] * quat[2], -sqx + sqy - sqz + sqw);
	return returnValues;
}

void Mesh::GetMesh(FbxScene* scene, std::string fileName)
{
	// From the Importer
	Importer importer;
	if (!importer.importFile(fileName))
	{
		FBXSDK_printf("Failed to Import the file");
	}
	// Get root node
	FbxNode* rootNode = scene->GetRootNode();

	const ImporterTransform * importedTransforms = importer.getTransform();
	for (unsigned int i = 0; i < importer.getNumTransforms(); i++)
	{
		FbxNode* TransformNode = FbxNode::Create(scene, importedTransforms[i].name);
	
		FbxQuaternion* quaternion;
		quaternion->Set(importedTransforms[i].rotation[0], importedTransforms[i].rotation[1], importedTransforms[i].rotation[2], importedTransforms[i].rotation[3]);
	
		std::cout << "iport rot" << std::endl;
		std::cout << importedTransforms[i].rotation[0] << std::endl;
		std::cout << importedTransforms[i].rotation[1] << std::endl;
		std::cout << importedTransforms[i].rotation[2] << std::endl;
		std::cout << importedTransforms[i].rotation[3] << std::endl;
		//double* euler = QuatToEuler(importedTransforms[i].rotation);
		TransformNode->LclRotation.Set(quaternion->DecomposeSphericalXYZ());

		TransformNode->LclScaling.Set(FbxDouble3(importedTransforms[i].scale[0], importedTransforms[i].scale[1], importedTransforms[i].scale[2]));

		TransformNode->LclTranslation.Set(FbxDouble3(importedTransforms[i].position[0], importedTransforms[i].position[1], importedTransforms[i].position[2]));

		int parentID = importedTransforms[i].parentID;
		//std::cout << parentID << std::endl;
		std::cout << "Rot we got" << std::endl;
		//std::cout << euler[0] << std::endl;
		//std::cout << euler[1] << std::endl;
		//std::cout << euler[2] << std::endl;
		//std::cout << TransformNode->LclRotation.Get()[0] << std::endl; 
		//std::cout << TransformNode->LclRotation.Get()[1] << std::endl; 
		//std::cout << TransformNode->LclRotation.Get()[2] << std::endl;
		//std::cout << TransformNode->LclScaling.Get()[0] << std::endl; 
		//std::cout << TransformNode->LclScaling.Get()[1] << std::endl; 
		//std::cout << TransformNode->LclScaling.Get()[2] << std::endl;
		std::cout << TransformNode->GetName() << std::endl;

		if (parentID < 0)
		{
			rootNode->AddChild(TransformNode);
		}
		else
		{
			FbxNode* parentNode = scene->GetNode(parentID);
			parentNode->AddChild(TransformNode);
		}
		//std::cout << ohkjewfd << std::endl;
	}
	std::cout << "TRANSFORM END" << std::endl;
	// MeshNode
	
	// Mesh
	std::vector<FbxMesh*> meshes;
	meshes.resize(importer.getNumMeshes());
	const ImporterMesh* importedMeshes = importer.getMesh();


	for (unsigned int i = 0; i < importer.getNumMeshes(); i++)
	{
		// Mesh
		FbxMesh* mesh = FbxMesh::Create(scene, importedMeshes[i].name);


		std::vector <FbxVector4> vertices;

		for (unsigned int j = 0; j < importedMeshes[i].indice_count; j++)
		{
			FbxVector4 vertex(importedMeshes[i].position[importedMeshes[i].vertices[j].position * 3 + 0], importedMeshes[i].position[importedMeshes[i].vertices[j].position * 3 + 1], importedMeshes[i].position[importedMeshes[i].vertices[j].position * 3 + 2]);
			vertices.push_back(vertex);
		}
		//meshes.push_back(mesh);
		
		mesh->InitControlPoints(importedMeshes[i].indice_count);
		FbxVector4* controlPoints = mesh->GetControlPoints();

		// Define Faces
		for (unsigned int j = 0; j < importedMeshes[i].indice_count; j++)
		{
			controlPoints[j] = vertices[j];
		}
		// Add Polygons to mesh
		for (unsigned int offset = 0; offset < importedMeshes[i].indice_count; offset += 3)
		{
			mesh->BeginPolygon();
			mesh->AddPolygon(offset + 0);
			mesh->AddPolygon(offset + 1);
			mesh->AddPolygon(offset + 2);
			mesh->EndPolygon();
		}

		FbxLayer* layer = mesh->GetLayer(0);
		if (layer == NULL)
		{
			mesh->CreateLayer();
			layer = mesh->GetLayer(0);
		}

		FbxLayerElementNormal* layerElementNormal = FbxLayerElementNormal::Create(mesh, "");
		layerElementNormal->SetMappingMode(FbxLayerElement::eByControlPoint);

		layerElementNormal->SetReferenceMode(FbxLayerElement::eDirect);

		for (unsigned int j = 0; j < importedMeshes[i].indice_count; j++)
		{
			layerElementNormal->GetDirectArray().Add(FbxVector4(importedMeshes[i].normal[importedMeshes[i].vertices[j].normal * 3 + 0], importedMeshes[i].normal[importedMeshes[i].vertices[j].normal * 3 + 1], importedMeshes[i].normal[importedMeshes[i].vertices[j].normal * 3 + 2]));
		}
		layer->SetNormals(layerElementNormal);

		std::cout << "MESH: " << mesh->GetName() << std::endl;
		for (int j = 0; j < importedMeshes[i].transform_count; j++)
		{
			
			int parentID = importedMeshes[i].transform_Id[j] + 1;
			std::cout << parentID << std::endl;

			//FbxNode* meshNode = FbxNode::Create(scene, importedMeshes[i].name);

			//meshNode->LclTranslation.Set(FbxVector4(importedTransforms[parentID].position[0], importedTransforms[parentID].position[1], importedTransforms[parentID].position[2]));
			//meshNode->LclRotation.Set(FbxVector4(importedTransforms[parentID].rotation[0], importedTransforms[parentID].rotation[1], importedTransforms[parentID].rotation[2]));
			//meshNode->LclScaling.Set(FbxVector4(importedTransforms[parentID].scale[0], importedTransforms[parentID].scale[1], importedTransforms[parentID].scale[2]));
			std::cout << scene->GetNodeCount() << std::endl;
			FbxNode* parentNode = scene->GetNode(parentID);
				//parentNode->AddChild(meshNode);
			parentNode->SetNodeAttribute(mesh);
			parentNode->GetName();
			std::cout << "Transform name : " << parentNode->GetName() << std::endl;
		}
	}
}

Mesh::~Mesh()
{

}