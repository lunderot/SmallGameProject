#include "Mesh.h"
#include <DirectXMath.h>

using namespace fbxsdk;
double* QuatToEuler(const double quat[4])
{
	double returnValues[3];

	//double sqw = quat[3] * quat[3];
	double sqx = quat[0] * quat[0];
	double sqy = quat[1] * quat[1];
	double sqz = quat[2] * quat[2];
	//double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	double test = quat[0] * quat[1] + quat[2] * quat[3];
	if (test > 0.49999999999) { // singularity at north pole
		returnValues[1] = 2 * atan2(quat[0], quat[3]) * 180 / FBXSDK_PI;
		returnValues[2] = FBXSDK_PI / 2 * 180 / FBXSDK_PI;
		returnValues[0] = 0;
		return returnValues;
	}
	if (test < -0.4999999999) { // singularity at south pole
		returnValues[1] = -2 * atan2(quat[0], quat[3]) * 180 / FBXSDK_PI;
		returnValues[2] = FBXSDK_PI / 2 * 180 / FBXSDK_PI;
		returnValues[0] = 0;
		return returnValues;
	}
	returnValues[1] = atan2(2 * quat[1] * quat[3] - 2 * quat[0] * quat[2], 1 - 2 * sqy - 2 * sqz) * 180 / FBXSDK_PI;//sqx - sqy - sqz + sqw);
	returnValues[2] = asin(2 * test) * 180 / FBXSDK_PI;
	returnValues[0] = atan2(2 * quat[0] * quat[3] - 2 * quat[1] * quat[2], 1 - 2 * sqx - 2 * sqz) * 180 / FBXSDK_PI;//-sqx + sqy - sqz + sqw);
	return returnValues;
}

void Mesh::ExportMeshes(FbxScene* scene, std::string fileName)
{
	// From the Importer
	Importer importer;
	if (!importer.importFile(fileName))
	{
		FBXSDK_printf("Failed to Import the file");
	}

	//Materials
	Material material;
	// Get root node
	FbxNode* rootNode = scene->GetRootNode();
	int counter = 0;
	const ImporterTransform * importedTransforms = importer.getTransform();
	std::cout << "TRANSFORMS" << endl;
	for (unsigned int i = 0; i < importer.getNumTransforms(); i++)
	{
		//FbxNode* TransformNode = FbxNode::Create(scene, std::string(std::string(importedTransforms[i].name) + std::to_string(counter)).c_str());
		FbxNode* TransformNode = FbxNode::Create(scene, importedTransforms[i].name);
		counter++;
		DirectX::XMVECTOR quat;
		quat = DirectX::XMVectorSet(importedTransforms[i].rotation[0], importedTransforms[i].rotation[1], importedTransforms[i].rotation[2], -importedTransforms[i].rotation[3]);
		DirectX::XMMATRIX quatMat;
		quatMat = DirectX::XMMatrixRotationQuaternion(quat);
		DirectX::XMFLOAT4X4 test;
		DirectX::XMStoreFloat4x4(&test, quatMat);
		//cout << test._32 << " " << test._33 << endl;
		//cout << test._31 << endl;
		//cout << test._21 << " " << test._11 << endl;
		FbxQuaternion* quaternion;
		//quaternion->Set(importedTransforms[i].rotation[0], importedTransforms[i].rotation[1], importedTransforms[i].rotation[2], importedTransforms[i].rotation[3]);
		double* Euler;
		Euler = QuatToEuler(importedTransforms[i].rotation);
		FbxMatrix matrix;
		
		//std::cout << "iport rot" << std::endl;
		//std::cout << importedTransforms[i].rotation[0] << std::endl;
		//std::cout << importedTransforms[i].rotation[1] << std::endl;
		//std::cout << importedTransforms[i].rotation[2] << std::endl;
		//std::cout << importedTransforms[i].rotation[3] << std::endl;
		//double* euler = QuatToEuler(importedTransforms[i].rotation);
		float tmp;
		cout <<  "cp" << abs(asin(-test._31)) << endl;
		if (abs(asin(-test._31)) < 0.001f)
			tmp = asin(-test._31) * 180 / FBXSDK_PI;
		else
			tmp = asin(-test._31) * 180 / FBXSDK_PI - 90;

		cout << tmp << endl;
		TransformNode->LclRotation.Set(FbxDouble3(atan(test._32 / test._33) * 180 / FBXSDK_PI, tmp, atan(test._21 / test._11) * 180 / FBXSDK_PI)/*quaternion->DecomposeSphericalXYZ()*/);

		TransformNode->LclScaling.Set(FbxDouble3(importedTransforms[i].scale[0], importedTransforms[i].scale[1], importedTransforms[i].scale[2]));

		TransformNode->LclTranslation.Set(FbxDouble3(importedTransforms[i].position[0], importedTransforms[i].position[1], importedTransforms[i].position[2]));

		TransformNode->SetPreRotation(FbxNode::EPivotSet::eSourcePivot, FbxVector4(0, 90, 0));
		TransformNode->SetPostRotation(FbxNode::EPivotSet::eSourcePivot, FbxVector4(0, -90, 0));
		int parentID = importedTransforms[i].parentID;
		//std::cout << parentID << std::endl;
		//std::cout << "Rot we got" << std::endl;
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
		unsigned int counter = 0;
		if (parentID < 0)
		{
			rootNode->AddChild(TransformNode);
			//std::cout << rootNode->GetChildCount(TransformNode) << endl;
		}
		else
		{
			FbxNode* parentNode = scene->GetNode(parentID + 1);
			//std::cout << "PARENTNODE: " << parentNode->GetChildCount() << endl;
			parentNode->AddChild(TransformNode);


		}
		//std::cout << ohkjewfd << std::endl;
	}
	//std::cout << "TRANSFORM END" << std::endl;
	// MeshNode

	// Mesh
	std::vector<FbxMesh*> meshes;
	meshes.resize(importer.getNumMeshes());
	const ImporterMesh* importedMeshes = importer.getMesh();
	const ImporterMaterial* importedMaterials = importer.getMaterial();
	std::cout << "MESHES" << endl;
	std::cout << importer.getNumMeshes() << endl;
	for (unsigned int i = 0; i < importer.getNumMeshes(); i++)
	{

		// Mesh
		cout << "MESH NAME: " << importedMeshes[i].name << endl;
		//FbxMesh* mesh = FbxMesh::Create(scene, std::string(std::string(importedMeshes[i].name) + std::to_string(counter)).c_str());
		FbxMesh* mesh = FbxMesh::Create(scene, importedMeshes[i].name);
		counter++;

		std::vector <FbxVector4> vertices;
		
		cout << "COUNTER: " << i << endl;
		cout << "INDICES: " << importedMeshes[i].indice_count << endl;
		cout << "VERTEX: " << importedMeshes[i].vertex_count << endl;
		//getchar();
		for (unsigned int j = 0; j < importedMeshes[i].vertex_count; j++)
		{
			cout << j << endl;
			//cout << importedMeshes[i].position[j * 3 + 0] << " " << importedMeshes[i].position[j * 3 + 1] << " " << importedMeshes[i].position[j * 3 + 2] << endl;
			FbxVector4 vertex(importedMeshes[i].position[j * 3 + 0], importedMeshes[i].position[j * 3 + 1], importedMeshes[i].position[j * 3 + 2]);
			vertices.push_back(vertex);
		}
		//meshes.push_back(mesh);

		mesh->InitControlPoints(importedMeshes[i].indice_count);
		FbxVector4* controlPoints = mesh->GetControlPoints();

		// Define Faces
		for (unsigned int j = 0; j < mesh->mControlPoints.Size(); j++)
		{
			controlPoints[j] = vertices[j];
		}

		// UV's
		FbxGeometryElementUV* uvDiffuseElement = mesh->CreateElementUV("DiffuseUV");
		uvDiffuseElement->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
		uvDiffuseElement->SetReferenceMode(FbxGeometryElement::eDirect);

		for (unsigned int j = 0; j < mesh->mControlPoints.Size(); j++)
		{
			uvDiffuseElement->GetDirectArray().Add(FbxVector2(importedMeshes[i].uv[importedMeshes[i].vertices[j].uv * 2 + 0], importedMeshes[i].uv[importedMeshes[i].vertices[j].uv * 2 + 1]));
		}

		uvDiffuseElement->GetIndexArray().SetCount(importedMeshes[i].uv_count);
		std::cout << "UV COUNT!!!!!!!!!!!!!!!!!!!!!!" << importedMeshes[i].uv_count << std::endl;

		vector<unsigned int> indices;
		for (unsigned int x = 0; x < importedMeshes[i].indice_count; x++)
		{
			indices.push_back(importedMeshes[i].vertices[x].position);
		}

		// Add Polygons to mesh
		for (unsigned int offset = 0; offset < mesh->mControlPoints.Size() / 3; offset++)
		{
			mesh->BeginPolygon(-1, -1, false);

			for (unsigned int j = 0; j < 3; j++)
			{
				mesh->AddPolygon(indices[offset * 3 + j]);
				uvDiffuseElement->GetIndexArray().SetAt(offset + j, offset + j);
			}
			mesh->EndPolygon();
		}

		mesh->BuildMeshEdgeArray();

		FbxLayer* layer = mesh->GetLayer(0);
		if (layer == NULL)
		{
			mesh->CreateLayer();
			layer = mesh->GetLayer(0);
		}

		FbxLayerElementNormal* layerElementNormal = FbxLayerElementNormal::Create(mesh, "");
		layerElementNormal->SetMappingMode(FbxLayerElement::eByPolygonVertex);

		layerElementNormal->SetReferenceMode(FbxLayerElement::eDirect);
		std::cout << "NORMAL COUNT" << importedMeshes[i].normal_count << std::endl;
		for (unsigned int j = 0; j < mesh->mControlPoints.Size(); j++)
		{
			layerElementNormal->GetDirectArray().Add(FbxVector4(importedMeshes[i].normal[importedMeshes[i].vertices[j].normal * 3 + 0], importedMeshes[i].normal[importedMeshes[i].vertices[j].normal * 3 + 1], importedMeshes[i].normal[importedMeshes[i].vertices[j].normal * 3 + 2]));
		}
		layer->SetNormals(layerElementNormal);

		//std::cout << "MESH: " << mesh->GetName() << std::endl;
		//std::cout << "TRANSFORM COUNT: " << importedMeshes[i].transform_count << std::endl;
		for (int j = 0; j < importedMeshes[i].transform_count; j++)
		{

			int parentID = importedMeshes[i].transform_Id[j] + 1;
			//std::cout << parentID << std::endl;

			//FbxNode* meshNode = FbxNode::Create(scene, importedMeshes[i].name);

			//meshNode->LclTranslation.Set(FbxVector4(importedTransforms[parentID].position[0], importedTransforms[parentID].position[1], importedTransforms[parentID].position[2]));
			//meshNode->LclRotation.Set(FbxVector4(importedTransforms[parentID].rotation[0], importedTransforms[parentID].rotation[1], importedTransforms[parentID].rotation[2]));
			//meshNode->LclScaling.Set(FbxVector4(importedTransforms[parentID].scale[0], importedTransforms[parentID].scale[1], importedTransforms[parentID].scale[2]));
			//std::cout << scene->GetNodeCount() << std::endl;
			FbxNode* parentNode = scene->GetNode(parentID);
			//parentNode->AddChild(meshNode);
			parentNode->SetNodeAttribute(mesh);
			parentNode->GetName();
			//std::cout << "Transform name : " << parentNode->GetName() << std::endl;
		}

		// Get Materials
		material.ExportMaterials(scene, mesh, importedMeshes[i], importedMaterials);
		//std::cout << "Mesh Export Done" << endl;
	}
}

Mesh::~Mesh()
{

}