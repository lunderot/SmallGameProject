#include "Importer.h"
#include <fstream>
int main()
{
	Importer importer;
	importer.importFile("C://test.bin");

	ofstream out("C://ImporterTest.txt");
	out << importer.headers << endl;

	for (unsigned int i = 0; i < importer.headers.material_count; i++)
		out << importer.materialHeaders[i] << endl;

	for (unsigned int i = 0; i < importer.headers.group_count; i++)
		out << importer.transformHeaders[i] << endl;

	for (unsigned int i = 0; i < importer.headers.camera_count; i++)
		out << importer.cameraHeaders[i] << endl;

	for (unsigned int i = 0; i < importer.headers.material_count; i++)
		out << importer.materials[i] << endl;

	for (unsigned int i = 0; i < importer.headers.group_count; i++)
		out << importer.transforms[i] << endl;

	for (unsigned int i = 0; i < importer.headers.camera_count; i++)
		out << importer.cameras[i] << endl;

	for (unsigned int i = 0; i < importer.headers.material_count; i++)
		out << importer.meshes[i] << endl;

	out << "getNumModels(): " << importer.getNumModels() << endl;
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

	return 0;
}