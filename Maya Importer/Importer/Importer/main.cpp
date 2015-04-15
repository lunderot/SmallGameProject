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

	return 0;
}