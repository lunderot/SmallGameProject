#include "Importer.h"
#include <fstream>
#include <iostream>
#include <DirectXMath.h>

Importer::Importer()
{
	transforms = nullptr;
	meshes = nullptr;
	cameras = nullptr;
	materials = nullptr;
	lights = nullptr;
	nurbs = nullptr;
};

Importer::~Importer()
{
	delete[] transforms;
	delete[] meshes;
	delete[] cameras;
	delete[] materials;
	delete[] lights;
	delete[] nurbs;

	//delete[] spheres;
	//delete[] models;
	delete[] joints;
};

bool Importer::importFile(string filePathAndName)
{
	char* fileData;
	ifstream in(filePathAndName.c_str(), ios_base::binary | ios_base::ate);

	if (!in.is_open())
		return false;
	unsigned int fileByteSize = in.tellg();
	in.seekg(0, ios_base::beg);

	fileData = new char[fileByteSize];
	in.read(fileData, fileByteSize);
	in.close();

	unsigned int offset = 0;

	if (!extractMainHeader(offset, fileData, fileByteSize)) return false;

	if (!extractMaterials(offset, fileData, fileByteSize)) return false;

	if (!extractTransforms(offset, fileData, fileByteSize)) return false;

	if (!extractJoint(offset, fileData, fileByteSize)) return false;

	if (!extractCameras(offset, fileData, fileByteSize)) return false;

	if (!extractMeshes(offset, fileData, fileByteSize)) return false;

	if (!extractLights(offset, fileData, fileByteSize)) return false;

	if (!extractNurb(offset, fileData, fileByteSize)) return false;

	if (!extractMorphs(offset, fileData, fileByteSize)) return false;

	if (!extractSkinAnimations(offset, fileData, fileByteSize)) return false;

	if (!extractKeys(offset, fileData, fileByteSize)) return false;

	delete fileData;

	return true;
};

bool Importer::extractMainHeader(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	if (sizeof(Header) > fileSize - offset)
		return false;

	memcpy(&headers, &fileData[0], sizeof(Header));
	offset += sizeof(Header);

	return true;
}

bool Importer::extractTransforms(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	transforms = new Transform[headers.group_count];
	for (unsigned int i = 0; i < headers.group_count; i++)
	{
		if ((sizeof(Transform) - sizeof(char*)) > fileSize - offset)
			return false;

		memcpy(&transforms[i], &fileData[offset], sizeof(Transform) - sizeof(char*));
		offset += sizeof(Transform) - sizeof(char*) - 4;

		if (transforms[i].name_Length > fileSize - offset)
			return false;

		char* node_name = new char[transforms[i].name_Length + 1];
		node_name[transforms[i].name_Length] = '\0';
		transforms[i].name = node_name;

		memcpy((char*)transforms[i].name, &fileData[offset], transforms[i].name_Length);

		offset += transforms[i].name_Length;
	}

	return true;
}

bool Importer::extractMeshes(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	meshes = new meshStruct[headers.mesh_count];
	for (unsigned int i = 0; i < headers.mesh_count; i++)
	{
		memcpy(&meshes[i], &fileData[offset], sizeof(meshStruct) - sizeof(char*) * 9 - 3);
		offset += sizeof(meshStruct) - sizeof(char*) * 9 + 1;

		meshes[i].position = new double*[meshes[i].position_count];
		for (unsigned int j = 0; j < meshes[i].position_count; j++)
		{
			meshes[i].position[j] = new double[3];
			memcpy((char*)meshes[i].position[j], &fileData[offset], 3 * sizeof(double));
			offset += 3 * sizeof(double);
		}

		meshes[i].uv = new float*[meshes[i].uv_count];
		for (unsigned int j = 0; j < meshes[i].uv_count; j++)
		{
			meshes[i].uv[j] = new float[2];

			memcpy((char*)meshes[i].uv[j], &fileData[offset], 2 * sizeof(float));
			offset += 2 * sizeof(float);
		}

		meshes[i].normal = new double*[meshes[i].normal_count];
		for (unsigned int j = 0; j < meshes[i].normal_count; j++)
		{
			meshes[i].normal[j] = new double[3];

			memcpy((char*)meshes[i].normal[j], &fileData[offset], 3 * sizeof(double));
			offset += 3 * sizeof(double);
		}

		meshes[i].tangent = new double*[meshes[i].tangent_count];
		for (unsigned int j = 0; j < meshes[i].tangent_count; j++)
		{
			meshes[i].tangent[j] = new double[3];

			memcpy((char*)meshes[i].tangent[j], &fileData[offset], 3 * sizeof(double));
			offset += 3 * sizeof(double);
		}

		meshes[i].bi_tangent = new double*[meshes[i].biTangent_count];
		for (unsigned int j = 0; j < meshes[i].biTangent_count; j++)
		{
			meshes[i].bi_tangent[j] = new double[3];

			memcpy((char*)meshes[i].bi_tangent[j], &fileData[offset], 3 * sizeof(double));
			offset += 3 * sizeof(double);
		}

		meshes[i].vertices = new Vertex[meshes[i].indice_count];
		meshes[i].material_Id = new int[meshes[i].material_count];
		meshes[i].transform_Id = new int[meshes[i].transform_count];
		char* name = new char[meshes[i].name_length + 1];
		name[meshes[i].name_length] = '\0';

		memcpy((char*)meshes[i].transform_Id, &fileData[offset], meshes[i].transform_count * sizeof(int));
		offset += meshes[i].transform_count * sizeof(int);

		memcpy((char*)meshes[i].material_Id, &fileData[offset], meshes[i].material_count * sizeof(int));
		offset += meshes[i].material_count * sizeof(int);

		memcpy((char*)meshes[i].vertices, &fileData[offset], meshes[i].indice_count * sizeof(Vertex));
		offset += meshes[i].indice_count * sizeof(Vertex);

		memcpy((char*)name, &fileData[offset], meshes[i].name_length);
		meshes[i].name = name;
		offset += meshes[i].name_length;

	}

	return true;
}

bool Importer::extractCameras(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	cameras = new camera[headers.camera_count];
	for (unsigned int i = 0; i < headers.camera_count; i++)
	{
		memcpy(&cameras[i], &fileData[offset], sizeof(camera) - sizeof(char*) - sizeof(unsigned int*));
		offset += sizeof(camera) - sizeof(char*) - 4;

		cameras[i].parentID = new unsigned int[cameras[i].nrOfParents];
		memcpy(cameras[i].parentID, &fileData[offset], sizeof(int) * cameras[i].nrOfParents);
		offset += sizeof(unsigned int) * cameras[i].nrOfParents;

		char* node_name = new char[cameras[i].name_length + 1];
		node_name[cameras[i].name_length] = '\0';
		cameras[i].name = node_name;

		memcpy((char*)cameras[i].name, &fileData[offset], cameras[i].name_length);

		offset += cameras[i].name_length;

		cameras[i] = cameras[i];
	}

	return true;
}

bool Importer::extractMaterials(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	materials = new MaterialData[headers.material_count];
	for (unsigned int i = 0; i < headers.material_count; i++)
	{
		memcpy(&materials[i], &fileData[offset], sizeof(MaterialData) - sizeof(const char*) * 4);
		offset += sizeof(MaterialData) - sizeof(const char*) * 4;
		char* node_name = new char[materials[i].name_length + 1];
		node_name[materials[i].name_length] = '\0';

		materials[i].node_name = node_name;

		memcpy((char*)materials[i].node_name, &fileData[offset], materials[i].name_length);
		offset += materials[i].name_length;


		if (materials[i].duffuse_map_length != 0)
		{
			char* diffuse_map = new char[materials[i].duffuse_map_length + 1];
			diffuse_map[materials[i].duffuse_map_length] = '\0';

			memcpy(diffuse_map, &fileData[offset], materials[i].duffuse_map_length);
			offset += materials[i].duffuse_map_length;

			materials[i].diffuse_map = diffuse_map;
		}
		else
		{
			materials[i].diffuse_map = nullptr;
		}

		if (materials[i].normal_map_length != 0)
		{
			char* normal_map = new char[materials[i].normal_map_length + 1];
			normal_map[materials[i].normal_map_length] = '\0';

			memcpy(normal_map, &fileData[offset], materials[i].normal_map_length);
			offset += materials[i].normal_map_length;

			materials[i].normal_map = normal_map;
		}
		else
		{
			materials[i].normal_map = nullptr;
		}

		if (materials[i].specular_map_length != 0)
		{
			char* specular_map = new char[materials[i].specular_map_length + 1];
			specular_map[materials[i].specular_map_length] = '\0';

			memcpy(specular_map, &fileData[offset], materials[i].specular_map_length);
			offset += materials[i].specular_map_length;

			materials[i].specular_map = specular_map;
		}
		else
		{
			materials[i].specular_map = nullptr;
		}
	}

	return true;
}

bool Importer::extractLights(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	lights = new Light[headers.light_count];
	for (unsigned int i = 0; i < headers.light_count; i++)
	{
		memcpy(&lights[i], &fileData[offset], sizeof(Light) - sizeof(char*));
		offset += sizeof(Light) - sizeof(char*) - 4;

		char* name = new char[lights[i].name_Length + 1];
		name[lights[i].name_Length] = '\0';
		lights[i].name = name;

		memcpy((char*)lights[i].name, &fileData[offset], lights[i].name_Length);

		offset += lights[i].name_Length;
	}

	return true;
}

bool Importer::extractJoint(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	joints = new Joint[headers.joint_count];
	for (unsigned int i = 0; i < headers.joint_count; i++)
	{
		memcpy(&joints[i], &fileData[offset], sizeof(Joint) - sizeof(char*));
		offset += sizeof(Joint) - sizeof(char*) - 4;

		char* name = new char[joints[i].transform.name_Length + 1];
		name[joints[i].transform.name_Length] = '\0';
		joints[i].transform.name = name;

		memcpy((char*)joints[i].transform.name, &fileData[offset], joints[i].transform.name_Length);

		offset += joints[i].transform.name_Length;
	}

	return true;
}

bool Importer::extractNurb(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	nurbs = new Nurb[headers.nurb_count];
	for (unsigned int i = 0; i < headers.nurb_count; i++)
	{
		memcpy(&nurbs[i], &fileData[offset], sizeof(Nurb) - sizeof(char*) - sizeof(int*));
		offset += sizeof(Nurb) - sizeof(char*) - sizeof(int*) + 4;

		nurbs[i].parentID = new int[nurbs[i].numberOfParent];
		memcpy(nurbs[i].parentID, &fileData[offset], nurbs[i].numberOfParent * sizeof(int));
		offset += nurbs[i].numberOfParent * sizeof(int);

		char* name = new char[nurbs[i].name_Length + 1];
		name[nurbs[i].name_Length] = '\0';
		nurbs[i].name = name;

		memcpy((char*)nurbs[i].name, &fileData[offset], nurbs[i].name_Length);

		offset += nurbs[i].name_Length;
	}

	return true;
}

bool Importer::extractMorphs(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	morph = new MorphAnimation[headers.morph_count];
	for (unsigned int i = 0; i < headers.morph_count; i++)
	{
		memcpy(&morph[i], &fileData[offset], sizeof(MorphAnimation) - sizeof(char*) * 2);
		offset += sizeof(MorphAnimation) - sizeof(char*) * 2;

		morph[i].position = new double*[morph[i].nrOfPositions];
		for (unsigned int j = 0; j < morph[i].nrOfPositions; j++)
		{
			morph[i].position[j] = new double[3];
			memcpy((char*)morph[i].position[j], &fileData[offset], 3 * sizeof(double));
			offset += 3 * sizeof(double);
		}
		char* name = new char[morph[i].blendShape_name_length + 1];
		name[morph[i].blendShape_name_length] = '\0';
		memcpy((char*)name, &fileData[offset], morph[i].blendShape_name_length);
		morph[i].blendShapeName = name;
		offset += morph[i].blendShape_name_length;
	}
	return true;
}

bool Importer::extractSkinAnimations(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	skins = new SkinAnimation[headers.skin_count];
	for (unsigned int i = 0; i < headers.skin_count; i++)
	{
		memcpy(&skins[i], &fileData[offset], sizeof(SkinAnimation) - sizeof(char*) * 2);
		offset += sizeof(SkinAnimation) - sizeof(char*) * 2 + 4;
		
		skins[i].influenceIndices = new int[skins[i].numberOfInfluences];
		for (unsigned int j = 0; j < skins[i].numberOfInfluences; j++)
		{
			memcpy(&skins[i].influenceIndices[j], &fileData[offset], sizeof(int));
			offset += sizeof(int);
		}

		skins[i].influenceWeights = new VertexInfluence[skins[i].skinVertexCount];
		for (unsigned int j = 0; j < skins[i].skinVertexCount; j++)
		{
			memcpy(&skins[i].influenceWeights[j], &fileData[offset], sizeof(VertexInfluence));
			offset += sizeof(VertexInfluence);
		}

	}
	return true;
}

bool Importer::extractKeys(unsigned int& offset, char* fileData, unsigned int& fileSize)
{
	keys = new Keyframes[headers.anim_curve_count];
	for (unsigned int i = 0; i < headers.anim_curve_count; i++)
	{
		memcpy(&keys[i], &fileData[offset], sizeof(Keyframes) - sizeof(char*) * 3);
		offset += sizeof(Keyframes) - sizeof(char*) * 3;

		char* name = new char[keys[i].curveNameLength + 1];
		name[keys[i].curveNameLength] = '\0';
		memcpy(name, &fileData[offset], keys[i].curveNameLength);
		keys[i].curveName = name;
		offset += keys[i].curveNameLength;

		keys[i].attachToName = new char[keys[i].attachToNameLength + 1];
		keys[i].attachToName[keys[i].attachToNameLength] = '\0';
		memcpy(keys[i].attachToName, &fileData[offset], keys[i].attachToNameLength);
		offset += keys[i].attachToNameLength;

		keys[i].points = new KeyframePoint[keys[i].numberOfKeyframes];
		for (unsigned int j = 0; j < keys[i].numberOfKeyframes; j++)
		{
			memcpy(&keys[i].points[j], &fileData[offset], sizeof(KeyframePoint));
			offset += sizeof(KeyframePoint);
		}
	}
	return true;
}

unsigned int Importer::getNumTransforms() const
{
	return headers.group_count;
}

unsigned int Importer::getNumMaterials() const
{
	return headers.material_count;
}

unsigned int Importer::getNumCameras() const
{
	return headers.camera_count;
}

unsigned int Importer::getNumLights() const
{
	return headers.light_count;
}

unsigned int Importer::getNumMeshes() const
{
	return headers.mesh_count;
}

unsigned int Importer::getNumNurbs() const
{
	return headers.nurb_count;
}

const Transform* Importer::getTransform() const
{
	return transforms;
}

const meshStruct* Importer::getMesh() const
{
	return meshes;
}

const camera* Importer::getCamera() const
{
	return cameras;
}

const MaterialData* Importer::getMaterial() const
{
	return materials;
}

const Light* Importer::getLight() const
{
	return lights;
}

const Joint* Importer::getJoint() const
{
	return joints;
}

const Nurb* Importer::getNurb() const
{
	return nurbs;
}
