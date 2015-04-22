#include "Importer.h"
#include <fstream>
#include <iostream>
#include <DirectXMath.h>

Importer::Importer()
{
	meshGeometry = nullptr;
	transformHeaders = nullptr;
	meshHeaders = nullptr;
	cameraHeaders = nullptr;
	materialHeaders = nullptr;
	lightHeaders = nullptr;
	nurbHeaders = nullptr;

	transforms = nullptr;
	meshes = nullptr;
	cameras = nullptr;
	materials = nullptr;
	lights = nullptr;
	nurbs = nullptr;
};

Importer::~Importer()
{
	for (unsigned int i = 0; i < headers.mesh_count; i++)
		delete[] meshGeometry[i];
	delete[] meshGeometry;
	delete[] transformHeaders;
	delete[] meshHeaders;
	delete[] cameraHeaders;
	delete[] materialHeaders;
	delete[] lightHeaders;
	delete[] nurbHeaders;

	delete[] transforms;
	delete[] meshes;
	delete[] cameras;
	delete[] materials;
	delete[] lights;
	delete[] nurbs;
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

	if (!extractMainHeader(offset, fileData, fileByteSize)) return false;

	if (!extractMaterialHeader(offset, fileData, fileByteSize)) return false;

	if (!extractTransformHeader(offset, fileData, fileByteSize)) return false;

	if (!extractJointHeader(offset, fileData, fileByteSize)) return false;

	if (!extractCameraHeader(offset, fileData, fileByteSize)) return false;

	if (!extractMeshHeader(offset, fileData, fileByteSize)) return false;

	if (!extractLightHeader(offset, fileData, fileByteSize)) return false;

	if (!extractNurbHeader(offset, fileData, fileByteSize)) return false;

	if (!extractMaterials(offset, fileData, fileByteSize)) return false;

	if (!extractTransforms(offset, fileData, fileByteSize)) return false;

	if (!extractJoint(offset, fileData, fileByteSize)) return false;

	if (!extractCameras(offset, fileData, fileByteSize)) return false;

	if (!extractMeshes(offset, fileData, fileByteSize)) return false;

	if (!extractLights(offset, fileData, fileByteSize)) return false;

	if (!extractNurb(offset, fileData, fileByteSize)) return false;

	if (!constructVerticiesAndGeometry()) return false;

	if (!constructModels()) return false;

	if (!constructSpere()) return false;

	return true;
};

bool Importer::extractMainHeader(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	if (sizeof(Header) > fileSize - offset)
		return false;

	Header extractedHeader;
	memcpy(&extractedHeader, &fileData[0], sizeof(Header));
	offset += sizeof(Header);
	headers = extractedHeader;

	return true;
}

bool Importer::extractTransformHeader(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	if (sizeof(TransformHeader) * headers.group_count > fileSize - offset)
		return false;

	transformHeaders = new TransformHeader[headers.group_count];
	for (unsigned int i = 0; i < headers.group_count; i++)
	{
		TransformHeader extractedTransformHeader;
		memcpy(&extractedTransformHeader, &fileData[offset], sizeof(TransformHeader));
		offset += sizeof(TransformHeader);
		transformHeaders[i] = extractedTransformHeader;
	}

	return true;
}

bool Importer::extractMeshHeader(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	if (sizeof(MeshHeader) * headers.mesh_count > fileSize - offset)
		return false;

	meshHeaders = new MeshHeader[headers.mesh_count];
	for (unsigned int i = 0; i < headers.mesh_count; i++)
	{
		MeshHeader extractedMesh;
		memcpy(&extractedMesh, &fileData[offset], sizeof(MeshHeader));
		offset += sizeof(MeshHeader);
		meshHeaders[i] = extractedMesh;
	}

	return true;
}

bool Importer::extractCameraHeader(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	if (sizeof(CameraHeader) * headers.camera_count > fileSize - offset)
		return false;

	cameraHeaders = new CameraHeader[headers.camera_count];
	for (unsigned int i = 0; i < headers.camera_count; i++)
	{
		CameraHeader extractedCamera;
		memcpy(&extractedCamera, &fileData[offset], sizeof(CameraHeader));
		offset += sizeof(CameraHeader);
		cameraHeaders[i] = extractedCamera;
	}

	return true;
}

bool Importer::extractMaterialHeader(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	if (sizeof(MaterialHeader) * headers.material_count > fileSize - offset)
		return false;

	materialHeaders = new MaterialHeader[headers.material_count];
	for (unsigned int i = 0; i < headers.material_count; i++)
	{
		MaterialHeader extractedMaterialHeader;
		memcpy(&extractedMaterialHeader, &fileData[offset], sizeof(MaterialHeader));
		offset += sizeof(MaterialHeader);
		materialHeaders[i] = extractedMaterialHeader;
	}
	
	return true;
}

bool Importer::extractLightHeader(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	if (sizeof(LightHeader) * headers.light_count > fileSize - offset)
		return false;

	lightHeaders = new LightHeader[headers.light_count];
	for (unsigned int i = 0; i < headers.light_count; i++)
	{
		LightHeader extractedLight;
		memcpy(&extractedLight, &fileData[offset], sizeof(LightHeader));
		offset += sizeof(LightHeader);
		lightHeaders[i] = extractedLight;
	}

	return true;
}

bool Importer::extractJointHeader(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	if (sizeof(JointHeader) * headers.joint_count > fileSize - offset)
		return false;

	jointHeaders = new JointHeader[headers.joint_count];
	for (unsigned int i = 0; i < headers.joint_count; i++)
	{	
		JointHeader extractesJoint;
		memcpy(&extractesJoint, &fileData[offset], sizeof(JointHeader));
		offset += sizeof(JointHeader);
		jointHeaders[i] = extractesJoint;
	}
	return true;
}

bool Importer::extractNurbHeader(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	if (sizeof(NurbHeader) * headers.nurb_count > fileSize - offset)
		return false;

	nurbHeaders = new NurbHeader[headers.nurb_count];
	for (unsigned int i = 0; i < headers.nurb_count; i++)
	{
		NurbHeader extractesNurb;
		memcpy(&extractesNurb, &fileData[offset], sizeof(NurbHeader));
		offset += sizeof(NurbHeader);
		nurbHeaders[i] = extractesNurb;
	}

	return true;
}

bool Importer::extractTransforms(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	unsigned int readSize = 0;
	for (unsigned int i = 0; i < headers.group_count; i++)
	{
		readSize += (sizeof(Transform) - sizeof(char*)) + transformHeaders[i].name_Length;
	}

	if (readSize > fileSize - offset)
		return false;

	transforms = new Transform[headers.group_count];
	for (unsigned int i = 0; i < headers.group_count; i++)
	{
		Transform extractedTransform;
		memcpy(&extractedTransform, &fileData[offset], sizeof(Transform) - sizeof(char*));
		offset += sizeof(Transform) - sizeof(char*) - 4;

		char* node_name = new char[transformHeaders[i].name_Length + 1];
		node_name[transformHeaders[i].name_Length] = '\0';
		extractedTransform.name = node_name;

		memcpy((char*)extractedTransform.name, &fileData[offset], transformHeaders[i].name_Length);

		offset += transformHeaders[i].name_Length;

		transforms[i] = extractedTransform;
	}

	return true;
}

bool Importer::extractMeshes(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	unsigned int readSize = 0;
	for (unsigned int i = 0; i < headers.mesh_count; i++)
	{
		readSize += meshHeaders[i].position_count * sizeof(double) * 3 + meshHeaders[i].uv_count * sizeof(float) + meshHeaders[i].normal_count * sizeof(double) * 3 +
					meshHeaders[i].tangent_count * sizeof(double) * 3 + meshHeaders[i].biTangent_count * sizeof(double) * 3 +
					meshHeaders[i].transform_count * sizeof(int) + meshHeaders[i].material_count * sizeof(int) + meshHeaders[i].indice_count * sizeof(Vertex) +
					meshHeaders[i].name_length;
	}

	if (readSize > fileSize - offset)
		return false;

	meshes = new meshStruct[headers.mesh_count];
	for (unsigned int i = 0; i < headers.mesh_count; i++)
	{

		meshStruct extractedMesh;

		extractedMesh.position.resize(meshHeaders[i].position_count);
		for (unsigned int j = 0; j < meshHeaders[i].position_count; j++)
		{
			extractedMesh.position[j].resize(3);

			memcpy((char*)extractedMesh.position[j].data(), &fileData[offset], extractedMesh.position[j].size() * sizeof(double));
			offset += extractedMesh.position[j].size() * sizeof(double);
		}

		extractedMesh.uv.resize(meshHeaders[i].uv_count);
		for (unsigned int j = 0; j < meshHeaders[i].uv_count; j++)
		{
			extractedMesh.uv[j].resize(2);

			memcpy((char*)extractedMesh.uv[j].data(), &fileData[offset], extractedMesh.uv[j].size() * sizeof(float));
			offset += extractedMesh.uv[j].size() * sizeof(float);
		}

		extractedMesh.normal.resize(meshHeaders[i].normal_count);
		for (unsigned int j = 0; j < meshHeaders[i].normal_count; j++)
		{
			extractedMesh.normal[j].resize(3);

			memcpy((char*)extractedMesh.normal[j].data(), &fileData[offset], extractedMesh.normal[j].size() * sizeof(double));
			offset += extractedMesh.normal[j].size() * sizeof(double);
		}

		extractedMesh.tangent.resize(meshHeaders[i].tangent_count);
		for (unsigned int j = 0; j < meshHeaders[i].tangent_count; j++)
		{
			extractedMesh.tangent[j].resize(3);

			memcpy((char*)extractedMesh.tangent[j].data(), &fileData[offset], extractedMesh.tangent[j].size() * sizeof(double));
			offset += extractedMesh.tangent[j].size() * sizeof(double);
		}

		extractedMesh.bi_tangent.resize(meshHeaders[i].biTangent_count);
		for (unsigned int j = 0; j < meshHeaders[i].biTangent_count; j++)
		{
			extractedMesh.bi_tangent[j].resize(3);

			memcpy((char*)extractedMesh.bi_tangent[j].data(), &fileData[offset], extractedMesh.bi_tangent[j].size() * sizeof(double));
			offset += extractedMesh.bi_tangent[j].size() * sizeof(double);
		}

		extractedMesh.vertices.resize(meshHeaders[i].indice_count);
		extractedMesh.material_Id.resize(meshHeaders[i].material_count);
		extractedMesh.transform_Id.resize(meshHeaders[i].transform_count);
		char* name = new char[meshHeaders[i].name_length + 1];
		name[meshHeaders[i].name_length] = '\0';

		memcpy((char*)extractedMesh.transform_Id.data(), &fileData[offset], meshHeaders[i].transform_count * sizeof(int));
		offset += meshHeaders[i].transform_count * sizeof(int);

		memcpy((char*)extractedMesh.material_Id.data(), &fileData[offset], meshHeaders[i].material_count * sizeof(int));
		offset += meshHeaders[i].material_count * sizeof(int);

		memcpy((char*)extractedMesh.vertices.data(), &fileData[offset], meshHeaders[i].indice_count * sizeof(Vertex));
		offset += meshHeaders[i].indice_count * sizeof(Vertex);

		memcpy((char*)name, &fileData[offset], meshHeaders[i].name_length);
		extractedMesh.name = name;
		offset += meshHeaders[i].name_length;

		meshes[i] = extractedMesh;
	}

	return true;
}

bool Importer::extractCameras(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	unsigned int readSize = 0;
	for (unsigned int i = 0; i < headers.camera_count; i++)
	{
		readSize += (sizeof(camera) - sizeof(char*) - sizeof(char*) + cameraHeaders[i].name_length + cameraHeaders[i].nrOfParents * sizeof(int));
	}

	if (readSize > fileSize - offset)
		return false;


	cameras = new camera[headers.camera_count];
	for (unsigned int i = 0; i < headers.camera_count; i++)
	{
		camera extractCamera;
		memcpy(&extractCamera, &fileData[offset], sizeof(camera) - sizeof(char*) - sizeof(unsigned int*));
		offset += sizeof(camera) - sizeof(char*) - 4;

		extractCamera.parentID = new unsigned int[cameraHeaders[i].nrOfParents];
		memcpy(extractCamera.parentID, &fileData[offset], sizeof(int) * cameraHeaders[i].nrOfParents);
		offset += sizeof(unsigned int) * cameraHeaders[i].nrOfParents;

		char* node_name = new char[cameraHeaders[i].name_length + 1];
		node_name[cameraHeaders[i].name_length] = '\0';
		extractCamera.name = node_name;

		memcpy((char*)extractCamera.name, &fileData[offset], cameraHeaders[i].name_length);

		offset += cameraHeaders[i].name_length;

		cameras[i] = extractCamera;
	}

	return true;
}

bool Importer::extractMaterials(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	unsigned int readSize = 0;
	for (unsigned int i = 0; i < headers.material_count; i++)
	{
		readSize += (sizeof(Material) - sizeof(char*) * 4) +
			materialHeaders[i].name_length + materialHeaders[i].duffuse_map_length +
			materialHeaders[i].normal_map_length + materialHeaders[i].specular_map_length;
	}

	if (readSize > fileSize - offset)
		return false;


	materials = new Material[headers.material_count];
	for (unsigned int i = 0; i < headers.material_count; i++)
	{
		Material extractedMaterial;
		memcpy(&extractedMaterial, &fileData[offset], sizeof(Material) - sizeof(const char*) * 4);
		offset += sizeof(Material) - sizeof(const char*) * 4;
		char* node_name = new char[materialHeaders[i].name_length + 1];
		node_name[materialHeaders[i].name_length] = '\0';

		extractedMaterial.node_name = node_name;

		memcpy((char*)extractedMaterial.node_name, &fileData[offset], materialHeaders[i].name_length);
		offset += materialHeaders[i].name_length;


		if (materialHeaders[i].duffuse_map_length != 0)
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

		if (materialHeaders[i].normal_map_length != 0)
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

		if (materialHeaders[i].specular_map_length != 0)
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

		materials[i] = extractedMaterial;
	}

	return true;
}

bool Importer::extractLights(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	unsigned int readSize = 0;
	for (unsigned int i = 0; i < headers.light_count; i++)
	{
		readSize += (sizeof(lights) - sizeof(char*)) + lightHeaders[i].name_Length;
	}

	if (readSize > fileSize - offset)
		return false;


	lights = new Light[headers.light_count];
	for (unsigned int i = 0; i < headers.light_count; i++)
	{
		Light extractLight;
		memcpy(&extractLight, &fileData[offset], sizeof(Light) - sizeof(char*));
		offset += sizeof(Light) - sizeof(char*) - 4;

		char* name = new char[lightHeaders[i].name_Length + 1];
		name[lightHeaders[i].name_Length] = '\0';
		extractLight.name = name;

		memcpy((char*)extractLight.name, &fileData[offset], lightHeaders[i].name_Length);

		offset += lightHeaders[i].name_Length;

		lights[i] = extractLight;
	}

	return true;
}

bool Importer::extractJoint(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	unsigned int readSize = 0;
	for (unsigned int i = 0; i < headers.light_count; i++)
	{
		readSize += (sizeof(Joint) - sizeof(char*) - 4) + jointHeaders[i].transformHeader.name_Length;
	}

	if (readSize > fileSize - offset)
		return false;

	joints = new Joint[headers.joint_count];
	for (unsigned int i = 0; i < headers.joint_count; i++)
	{
		Joint extractedJoint;
		memcpy(&extractedJoint, &fileData[offset], sizeof(Joint) - sizeof(char*));
		offset += sizeof(Joint) - sizeof(char*) - 4;

		char* name = new char[jointHeaders[i].transformHeader.name_Length + 1];
		name[jointHeaders[i].transformHeader.name_Length] = '\0';
		extractedJoint.transform.name = name;

		memcpy((char*)extractedJoint.transform.name, &fileData[offset], jointHeaders[i].transformHeader.name_Length);

		offset += jointHeaders[i].transformHeader.name_Length;

		joints[i] = extractedJoint;
	}

	return true;

}

bool Importer::extractNurb(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	unsigned int readSize = 0;
	for (unsigned int i = 0; i < headers.nurb_count; i++)
	{
		readSize += (sizeof(Nurb) - sizeof(char*) - sizeof(int*)) + nurbHeaders[i].name_Length + nurbHeaders[i].numberOfParent * sizeof(int);
	}

	if (readSize > fileSize - offset)
		return false;

	nurbs = new Nurb[headers.nurb_count];
	for (unsigned int i = 0; i < headers.nurb_count; i++)
	{
		Nurb extractedNurb;
		memcpy(&extractedNurb, &fileData[offset], sizeof(Nurb) - sizeof(char*) - sizeof(int*));
		offset += sizeof(Nurb) - sizeof(char*) - sizeof(int*);

		unsigned int test1 = sizeof(Nurb);
		unsigned int test2 = sizeof(char*);
		unsigned int test3 = sizeof(int*);
		unsigned int test4 = sizeof(float);

		extractedNurb.parentID = new int[nurbHeaders[i].numberOfParent];
		memcpy(extractedNurb.parentID, &fileData[offset], nurbHeaders[i].numberOfParent * sizeof(int));
		offset += nurbHeaders[i].numberOfParent * sizeof(int);

		char* name = new char[nurbHeaders[i].name_Length + 1];
		name[nurbHeaders[i].name_Length] = '\0';
		extractedNurb.name = name;

		memcpy((char*)extractedNurb.name, &fileData[offset], nurbHeaders[i].name_Length);

		offset += nurbHeaders[i].name_Length;

		nurbs[i] = extractedNurb;
	}

	return true;
}

bool Importer::constructVerticiesAndGeometry()
{
	meshGeometry = new VertexPositionTexCoordNormalBinormalTangent* [headers.mesh_count];

	for (unsigned int i = 0; i < headers.mesh_count; i++)
	{
		meshGeometry[i] = new VertexPositionTexCoordNormalBinormalTangent[meshHeaders[i].indice_count];
		for (unsigned int j = 0; j < meshHeaders[i].indice_count; j++)
		{
			unsigned int positionID = meshes[i].vertices[j].position;
			unsigned int uvID = meshes[i].vertices[j].uv;
			unsigned int normalID = meshes[i].vertices[j].normal;
			meshGeometry[i][j].position[0] = (float)meshes[i].position[positionID][0];
			meshGeometry[i][j].position[1] = (float)meshes[i].position[positionID][1];
			meshGeometry[i][j].position[2] = (float)meshes[i].position[positionID][2];
			memcpy(meshGeometry[i][j].texCoord, meshes[i].uv[uvID].data(), sizeof(float) * 2);
			meshGeometry[i][j].normal[0] = (float)meshes[i].normal[normalID][0];
			meshGeometry[i][j].normal[1] = (float)meshes[i].normal[normalID][1];
			meshGeometry[i][j].normal[2] = (float)meshes[i].normal[normalID][2];
			meshGeometry[i][j].biNormal[0] = (float)meshes[i].bi_tangent[normalID][0];
			meshGeometry[i][j].biNormal[1] = (float)meshes[i].bi_tangent[normalID][1];
			meshGeometry[i][j].biNormal[2] = (float)meshes[i].bi_tangent[normalID][2];
			meshGeometry[i][j].tangent[0] = (float)meshes[i].tangent[normalID][0];
			meshGeometry[i][j].tangent[1] = (float)meshes[i].tangent[normalID][1];
			meshGeometry[i][j].tangent[2] = (float)meshes[i].tangent[normalID][2];
		}
	}

	return true;
}

bool Importer::constructModels()
{	
	for (unsigned int i = 0; i < headers.mesh_count; i++)
		numModels += meshHeaders[i].transform_count;

	models = new Model[numModels];
	
	int modelID = 0;

	for (unsigned int i = 0; i < headers.mesh_count; i++)
	{
		for (unsigned int j = 0; j < meshHeaders[i].transform_count; j++)
		{
			int parent = meshes[i].transform_Id[j];

			while (parent != -1)
			{
				double pos[3];
				double rotD[4];
				double scale[3];

				memcpy(pos, transforms[parent].position, sizeof(double) * 3);
				memcpy(rotD, transforms[parent].rotation, sizeof(double) * 4);
				memcpy(scale, transforms[parent].scale, sizeof(double) * 3);

				float rot[4];

				rot[0] = (float)rotD[0];
				rot[1] = (float)rotD[1];
				rot[2] = (float)rotD[2];
				rot[3] = (float)rotD[3];


				models[modelID].position[0] += pos[0];
				models[modelID].position[1] += pos[1];
				models[modelID].position[2] += pos[2];

				DirectX::XMVECTOR Q1, Q2;
				Q1 = DirectX::XMVectorSet(rot[0], rot[1], rot[2], rot[3]);
				Q2 = DirectX::XMVectorSet(models[modelID].rotation[0], models[modelID].rotation[1], models[modelID].rotation[2], models[modelID].rotation[3]);

				Q2 = DirectX::XMQuaternionMultiply(Q1, Q2);


				DirectX::XMFLOAT4 tmp;
				DirectX::XMStoreFloat4(&tmp, Q2);
				models[modelID].rotation[0] = tmp.x;
				models[modelID].rotation[1] = tmp.y;
				models[modelID].rotation[2] = tmp.z;
				models[modelID].rotation[3] = tmp.w;


				models[modelID].scale[0] *= scale[0];
				models[modelID].scale[1] *= scale[1];
				models[modelID].scale[2] *= scale[2];

				parent = transforms[parent].parentID;
			}
			
			models[modelID].MeshID = i;
			models[modelID].MaterialID = j;

			modelID++;
		}
	}

	return true;
}

bool Importer::constructSpere()
{
	for (unsigned int i = 0; i < headers.nurb_count; i++)
		numSpheres += nurbHeaders[i].numberOfParent;

	spheres = new BoundingSphere[numSpheres];

	unsigned int sphereID = 0;

	for (unsigned int i = 0; i < headers.nurb_count; i++)
	{
		for (unsigned int j = 0; j < nurbHeaders[i].numberOfParent; j++)
		{
			int parent = nurbs[i].parentID[j];

			while (parent != -1)
			{
				double pos[3];

				memcpy(pos, transforms[parent].position, sizeof(double) * 3);

				spheres[sphereID].position[0] += pos[0];
				spheres[sphereID].position[1] += pos[1];
				spheres[sphereID].position[2] += pos[2];

				parent = transforms[parent].parentID;;
			}
			spheres[sphereID].radius = nurbs[i].radius;
			sphereID++;
		}
	}

	return true;
}

const Material* Importer::getMatrials() const
{
	return materials;
}

const VertexPositionTexCoordNormalBinormalTangent* Importer::getMesh(unsigned int meshID) const
{
	if (0 <= meshID && meshID < headers.mesh_count)
		return meshGeometry[meshID];
	else
		return nullptr;
}

unsigned int Importer::getNumBoundingSphere() const
{
	return numSpheres;
}

const BoundingSphere* Importer::getBoundingSphere() const
{
	if (numSpheres != 0)
		return spheres;
	else
		return nullptr;
}

const Model* Importer::getModels() const
{
	return models;
}

unsigned int Importer::getNumMaterials() const
{
	return headers.material_count;
}

unsigned int Importer::getNumMeshes() const
{
	return headers.mesh_count;
}

unsigned int Importer::getNumModels() const
{
	return numModels;
}

unsigned int Importer::getMeshVertexCount(unsigned int meshID) const
{
	if (0 <= meshID && meshID < headers.mesh_count)
		return meshHeaders[meshID].indice_count;
	else
		return 0;
}