#include "Importer.h"
#include <iostream>
#include <fstream>
int main()
{
	Importer importer;
	if (!importer.importFile("C://Users/Litet/Documents/GitHub/SmallGameProject/FBX Export/FBX Export/rekt.bin"))
	{
		std::cout << "IMPORTER FAIL!!!" << endl;
	}

	ofstream out("C://Users/Litet/Documents/GitHub/SmallGameProject/FBX Export/FBX Export/Portal_test_FBX_ImporterTest.txt");
	//out << importer.headers << endl;

	const ImporterTransform* transforms = importer.getTransform();
	for (unsigned int i = 0; i < importer.getNumTransforms(); i++)
	{
		out << transforms[i].name << endl;
		out << transforms[i].rotation[0] << " " << transforms[i].rotation[1] << " " << transforms[i].rotation[2] << " " << transforms[i].rotation[3] << " " << endl;
	}

	const ImporterMesh* meshes = importer.getMesh();
	for (unsigned int i = 0; i < importer.getNumMeshes(); i++)
	{
		out << meshes[i].position_count << endl;
		for (unsigned int j = 0; j < meshes[i].position_count; j++)
			out << meshes[i].position[j] << endl;

		for (unsigned int j = 0; j < meshes[i].uv_count; j++)
			out << meshes[i].uv[j] << endl;

		for (unsigned int j = 0; j < meshes[i].normal_count; j++)
			out << meshes[i].normal[j] << endl;

		for (unsigned int j = 0; j < meshes[i].indice_count; j++)
			out << meshes[i].vertices[j] << endl;
	}

	//for (unsigned int i = 0; i < importer.headers.joint_count; i++)
	//	out << importer.joints[i] << endl;

	//for (unsigned int i = 0; i < importer.headers.camera_count; i++)
	//	out << importer.cameras[i] << endl;

	//for (unsigned int i = 0; i < importer.headers.mesh_count; i++)
	//	out << importer.meshes[i] << endl;

	//for (unsigned int i = 0; i < importer.headers.light_count; i++)
	//	out << importer.lights[i] << endl;

	//for (unsigned int i = 0; i < importer.headers.nurb_count; i++)
	//	out << importer.nurbs[i] << endl;

	//for (unsigned int i = 0; i < importer.headers.morph_count; i++)
	//	out << importer.morph[i] << endl;

	//for (unsigned int i = 0; i < importer.headers.skin_count; i++)
	//	out << importer.skins[i] << endl;

	//for (unsigned int i = 0; i < importer.headers.anim_curve_count; i++)
	//	out << importer.getCurves()[i] << endl;

	//for (unsigned int i = 0; i < importer.headers.material_count; i++)
	//{
	//	out << importer.materials[i].name_length;
	//	out << importer.materials[i].duffuse_map_length;
	//	out << importer.materials[i].normal_map_length;
	//	out << importer.materials[i].specular_map_length;
	//	out << importer.materials[i].mtrl_type;
	//	out << importer.materials[i].normal_depth;
	//	out << importer.materials[i].specular[0];
	//	out << importer.materials[i].specular[1];
	//	out << importer.materials[i].specular[2];
	//	out << importer.materials[i].specular_factor;
	//	out << importer.materials[i].shininess;
	//	out << importer.materials[i].reflection[0];
	//	out << importer.materials[i].reflection[1];
	//	out << importer.materials[i].reflection[2];
	//	out << importer.materials[i].reflection_factor;
	//	out << importer.materials[i].ambient[0];
	//	out << importer.materials[i].ambient[1];
	//	out << importer.materials[i].ambient[2];
	//	out << importer.materials[i].diffuse[0];
	//	out << importer.materials[i].diffuse[1];
	//	out << importer.materials[i].diffuse[2];
	//	out << importer.materials[i].diffuse_factor;
	//	out << importer.materials[i].transparency_color[0];
	//	out << importer.materials[i].transparency_color[1];
	//	out << importer.materials[i].transparency_color[2];
	//	out << importer.materials[i].incandescence[0];
	//	out << importer.materials[i].incandescence[1];
	//	out << importer.materials[i].incandescence[2];
	//	out << importer.materials[i].name;
	//	//out << importer.materials[i].diffuse_map;
	//	//out << importer.materials[i].normal_map;
	//	//out << importer.materials[i].specular_map;
	//};


	//for (unsigned int i = 0; i < importer.headers.group_count; i++)
	//{
	//	out << importer.transforms[i].name_Length;
	//	out << importer.transforms[i].parentID;
	//	out << importer.transforms[i].position[0];
	//	out << importer.transforms[i].position[1];
	//	out << importer.transforms[i].position[2];
	//	out << importer.transforms[i].rotation[0];
	//	out << importer.transforms[i].rotation[1];
	//	out << importer.transforms[i].rotation[2];
	//	out << importer.transforms[i].rotation[3];
	//	out << importer.transforms[i].scale[0];
	//	out << importer.transforms[i].scale[1];
	//	out << importer.transforms[i].scale[2];
	//	out << importer.transforms[i].name;
	//};

	//struct ImporterJoint
	//{
	//	double jointOrientation[4];
	//	ImporterTransform transform;
	//};

	//struct ImporterMesh
	//{
	//	unsigned int name_length;
	//	unsigned int vertex_count;
	//	unsigned int indice_count;
	//	unsigned int position_count;
	//	unsigned int uv_count;
	//	unsigned int normal_count;
	//	unsigned int tangent_count;
	//	unsigned int biTangent_count;
	//	unsigned int material_count;
	//	unsigned int transform_count;
	//	bool has_Animation;

	//	double* position;
	//	float* uv;
	//	double* normal;
	//	double* tangent;
	//	double* bi_tangent;

	//	int* transform_Id;
	//	int* material_Id;
	//	Vertex* vertices;
	//	char*  name;
	//};

	//struct ImporterCamera
	//{
	//	unsigned int name_length;
	//	unsigned int nrOfParents;
	//	double position[3];
	//	double up_vector[3];
	//	double interest_position[3];
	//	double field_of_view_x;
	//	double field_of_view_y;
	//	double near_plane;
	//	double far_plane;
	//	projection_type projection;
	//	unsigned int* parentID;
	//	char*  name;
	//};

	//struct ImporterLight
	//{
	//	unsigned int name_Length;
	//	light_type type;
	//	double color[3];
	//	float intensity;
	//	decay_type dType;
	//	bool cast_shadows;
	//	double shadow_color[3];
	//	char* name;
	//};

	//struct ImporterNurb
	//{
	//	unsigned int name_Length;
	//	unsigned int numberOfParent;
	//	float radius;
	//	int* ParentID;
	//	char* name;
	//};

	//struct ImporterMorphAnimation
	//{
	//	unsigned int blendShape_name_length;
	//	unsigned int nrOfWeights;
	//	unsigned int nrOfTargets;
	//	unsigned int nrOfVertsPerMesh;
	//	unsigned int meshID;
	//	unsigned int nrOfPositions;

	//	double* positions;
	//	char* name;
	//};

	//struct ImporterWeights
	//{
	//	unsigned int numberOfInfluences;
	//	unsigned int skinMeshIndex;
	//	unsigned int skinVertexCount;
	//	int * influenceIndices;
	//	VertexInfluence* Weights;
	//};

	/*out << "getNumModels(): " << importer.getNumModels() << endl;
	out << "getNumMeshes(): " << importer.getNumMeshes() << endl;
	out << "getNumMaterials(): " << importer.getNumMaterials() << endl;
	out << "getModels(): " << endl;
	for (unsigned int i = 0; i < importer.getNumModels(); i++)
	{
	out << "Model " << i << ": " << endl;
	out << importer.getModels()[i].MaterialID << endl;
	out << importer.getModels()[i].MeshID << endl;
	out << importer.getModels()[i].position[0] << " " << importer.getModels()[i].position[1] << " " << importer.getModels()[i].position[2] << endl;
	out << importer.getModels()[i].rotation[0] << " " << importer.getModels()[i].rotation[1] << " " << importer.getModels()[i].rotation[2] << " " << importer.getModels()[i].rotation[3] << endl;
	out << importer.getModels()[i].scale[0] << " " << importer.getModels()[i].scale[1] << " " << importer.getModels()[i].scale[2] << endl;
	}

	for (unsigned int i = 0; i < importer.getNumMeshes(); i++)
	{
	out << "Mesh " << i << ": " << endl;
	for (unsigned int j = 0; j < importer.getMeshVertexCount(i); j++)
	{
	out << "Vertex " << j << ": " << endl;
	out << importer.getMesh(i)[j].position[0] << " " << importer.getMesh(i)[j].position[1] << " " << importer.getMesh(i)[j].position[2] << endl;
	out << importer.getMesh(i)[j].texCoord[0] << " " << importer.getMesh(i)[j].texCoord[1] << endl;
	out << importer.getMesh(i)[j].normal[0] << " " << importer.getMesh(i)[j].normal[1] << " " << importer.getMesh(i)[j].normal[2] << endl;
	out << importer.getMesh(i)[j].biNormal[0] << " " << importer.getMesh(i)[j].biNormal[1] << " " << importer.getMesh(i)[j].biNormal[2] << endl;
	out << importer.getMesh(i)[j].tangent[0] << " " << importer.getMesh(i)[j].tangent[1] << " " << importer.getMesh(i)[j].tangent[2] << endl;
	}
	}

	for (unsigned int i = 0; i < importer.getNumMaterials(); i++)
	{
	out << "Material " << i << ": " << endl;
	out << importer.getMatrials()[i].diffuse[0] << " " << importer.getMatrials()[i].diffuse[1] << " " << importer.getMatrials()[i].diffuse[2] << endl;
	out << importer.getMatrials()[i].diffuse_factor << endl;
	out << importer.getMatrials()[i].ambient[0] << " " << importer.getMatrials()[i].ambient[1] << " " << importer.getMatrials()[i].ambient[2] << endl;
	out << importer.getMatrials()[i].specular[0] << " " << importer.getMatrials()[i].specular[1] << " " << importer.getMatrials()[i].specular[2] << endl;
	out << importer.getMatrials()[i].specular_factor << endl;
	out << importer.getMatrials()[i].shininess << endl;
	out << importer.getMatrials()[i].incandescence[0] << " " << importer.getMatrials()[i].incandescence[1] << " " << importer.getMatrials()[i].incandescence[2] << endl;
	out << importer.getMatrials()[i].transparency_color[0] << " " << importer.getMatrials()[i].transparency_color[1] << " " << importer.getMatrials()[i].transparency_color[2] << endl;
	out << importer.getMatrials()[i].mtrl_type << endl;
	out << importer.getMatrials()[i].reflection[0] << " " << importer.getMatrials()[i].reflection[1] << " " << importer.getMatrials()[i].reflection[2] << endl;
	out << importer.getMatrials()[i].reflection_factor << endl;
	out << importer.getMatrials()[i].node_name << endl;
	if (importer.getMatrials()[i].normal_map != nullptr)
	out << importer.getMatrials()[i].normal_map << endl;
	out << importer.getMatrials()[i].normal_depth << endl;
	if (importer.getMatrials()[i].specular_map != nullptr)
	out << importer.getMatrials()[i].specular_map << endl;
	}

	out << "getNumBoundingSphere(): " << importer.getNumBoundingSphere() << endl;

	out << "getNumBoundingSphere(): " << endl;
	for (unsigned int i = 0; i < importer.getNumBoundingSphere(); i++)
	{
	out << "Sphere " << i << ": " << endl;
	out << importer.spheres[i].position[0] << " " << importer.spheres[i].position[1] << " " << importer.spheres[i].position[2] << endl;
	out << importer.spheres[i].radius << endl;
	}*/
	getchar();
	return 0;
}