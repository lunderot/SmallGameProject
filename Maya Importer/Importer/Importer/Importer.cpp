#include "Importer.h"
#include <fstream>
#include <iostream>
// magnus is here ;)
Importer::Importer()
{

};

Importer::~Importer()
{

};

bool Importer::importFile(string filePathAndName)
{
	char* fileData;
	ifstream in(filePathAndName.c_str(), ios_base::binary|ios_base::ate);
	if (!in.is_open())
		return false;
	unsigned int fileByteSize = in.tellg();
	in.seekg(0, ios_base::beg);

	fileData = new char[fileByteSize];
	in.read(fileData, fileByteSize);
	in.close();

	unsigned int offset = 0;

	Header extractedHeader;
	memcpy(&extractedHeader, &fileData[0], sizeof(Header));
	offset += sizeof(Header);
	headers = extractedHeader;


	for (unsigned int i = 0; i < extractedHeader.material_count; i++)
	{  
		MaterialHeader extractedMaterialHeader;
		memcpy(&extractedMaterialHeader, &fileData[offset], sizeof(MaterialHeader));
		offset += sizeof(MaterialHeader);
		materialHeaders.push_back(extractedMaterialHeader);

		if (extractedMaterialHeader.name_length <= 0)
			return false;
	}

	
	for (unsigned int i = 0; i < extractedHeader.group_count; i++)
	{	
		TransformHeader extractedTransformHeader;
		memcpy(&extractedTransformHeader, &fileData[offset], sizeof(TransformHeader));
		offset += sizeof(TransformHeader);
		transformHeaders.push_back(extractedTransformHeader);

		if (extractedTransformHeader.name_Length <= 0)
			return false;
	}

	
	for (unsigned int i = 0; i < extractedHeader.camera_count; i++)
	{	
		CameraHeader extractedCamera;
		memcpy(&extractedCamera, &fileData[offset], sizeof(CameraHeader));
		offset += sizeof(CameraHeader);
		cameraHeaders.push_back(extractedCamera);

		if (extractedCamera.name_length <= 0)
			return false;
	}


	for (unsigned int i = 0; i < extractedHeader.mesh_count; i++)
	{
		MeshHeader extractedMesh;
		memcpy(&extractedMesh, &fileData[offset], sizeof(MeshHeader));
		offset += sizeof(MeshHeader);
		meshHeaders.push_back(extractedMesh);

		if (extractedMesh.name_length <= 0)
			return false;
	}

	
	for (unsigned int i = 0; i < extractedHeader.material_count; i++)
	{	
		Material extractedMaterial;
		memcpy(&extractedMaterial, &fileData[offset], sizeof(Material) - sizeof(const char*) * 4);
		offset += sizeof(Material) - sizeof(const char*) * 4;
		char* node_name = new char[materialHeaders[i].name_length + 1];
		node_name[materialHeaders[i].name_length] = '\0';

		extractedMaterial.node_name = node_name;

		memcpy((char*)extractedMaterial.node_name, &fileData[offset], materialHeaders[i].name_length);
		offset += materialHeaders[i].name_length;
		

		if (materialHeaders[i].duffuse_map_length == 0)
		{
			char* diffuse_map = new char[materialHeaders[i].duffuse_map_length + 1];
			diffuse_map[materialHeaders[i].duffuse_map_length] = '\0';

			memcpy(diffuse_map, &fileData[offset], materialHeaders[i].duffuse_map_length);
			offset += materialHeaders[i].duffuse_map_length;

			extractedMaterial.diffuse_map = diffuse_map;
		}
		else
		{
			extractedMaterial.diffuse_map = nullptr;
		}

		if (materialHeaders[i].normal_map_length == 0)
		{
			char* normal_map = new char[materialHeaders[i].normal_map_length + 1];
			normal_map[materialHeaders[i].normal_map_length] = '\0';

			memcpy(normal_map, &fileData[offset], materialHeaders[i].normal_map_length);
			offset += materialHeaders[i].normal_map_length;

			extractedMaterial.normal_map = normal_map;
		}
		else
		{
			extractedMaterial.normal_map = nullptr;
		}

		if (materialHeaders[i].specular_map_length == 0)
		{
			char* specular_map = new char[materialHeaders[i].specular_map_length + 1];
			specular_map[materialHeaders[i].specular_map_length] = '\0';

			memcpy(specular_map, &fileData[offset], materialHeaders[i].specular_map_length);
			offset += materialHeaders[i].specular_map_length;

			extractedMaterial.specular_map = specular_map;
		}
		else
		{
			extractedMaterial.specular_map = nullptr;
		}

		materials.push_back(extractedMaterial);
	}


	for (unsigned int i = 0; i < extractedHeader.group_count; i++)
	{	
		Transform extractedTransform;
		memcpy(&extractedTransform, &fileData[offset], sizeof(Transform) - sizeof(char*));
		offset += sizeof(Transform) - sizeof(char*) - 4;

		char* node_name = new char[transformHeaders[i].name_Length + 1];
		node_name[transformHeaders[i].name_Length] = '\0';
		extractedTransform.name = node_name; 

		memcpy((char*)extractedTransform.name, &fileData[offset], transformHeaders[i].name_Length);

		offset += transformHeaders[i].name_Length;

		transforms.push_back(extractedTransform);
	}

	
	for (unsigned int i = 0; i < extractedHeader.camera_count; i++)
	{	
		camera extractCamera;
		memcpy(&extractCamera, &fileData[offset], sizeof(camera) - sizeof(char*));
		offset += sizeof(camera) - sizeof(char*) + 4;

		char* node_name = new char[cameraHeaders[i].name_length + 1];
		node_name[cameraHeaders[i].name_length] = '\0';
		extractCamera.name = node_name;

		memcpy((char*)extractCamera.name, &fileData[offset], cameraHeaders[i].name_length);

		offset += cameraHeaders[i].name_length;

		cameras.push_back(extractCamera);
	}


	for (unsigned int i = 0; i < extractedHeader.mesh_count; i++)
	{																						
		meshStruct extractedMesh;
		extractedMesh.vertices.resize(meshHeaders[i].vertex_count);
		extractedMesh.indices.resize(meshHeaders[i].triangle_count);
		extractedMesh.material_Id.resize(meshHeaders[i].material_count);
		extractedMesh.transform_Id.resize(meshHeaders[i].transform_count);

		memcpy((char*)extractedMesh.transform_Id.data(), &fileData[offset], meshHeaders[i].transform_count * sizeof(int));
		offset += meshHeaders[i].transform_count * sizeof(int);

		memcpy((char*)extractedMesh.material_Id.data(), &fileData[offset], meshHeaders[i].material_count * sizeof(int));
		offset += meshHeaders[i].material_count * sizeof(int);

		memcpy((char*)extractedMesh.vertices.data(), &fileData[offset], meshHeaders[i].vertex_count * sizeof(Vertex));
		offset += meshHeaders[i].vertex_count * sizeof(int);

		memcpy((char*)extractedMesh.indices.data(), &fileData[offset], meshHeaders[i].triangle_count * sizeof(int));
		offset += meshHeaders[i].triangle_count * sizeof(int);

		memcpy((char*)extractedMesh.name, &fileData[offset], meshHeaders[i].name_length);
		offset += meshHeaders[i].name_length;

		meshes.push_back(extractedMesh);
	}

	//ofstream out("C://ImporterTest.txt", ofstream::ate);
	//out << realHeader << endl;
	//out.close();
	return true;
};