#include "Importer.h"
#include <fstream>
int main()
{
	Importer importer;
	importer.importFile("C://test.bin");

	ofstream out("C://ImporterTest.txt");
	out << importer.headers << endl;

	for (unsigned int i = 0; i < importer.materialHeaders.size(); i++)
		out << importer.materialHeaders[i] << endl;

	for (unsigned int i = 0; i < importer.transformHeaders.size(); i++)
		out << importer.transformHeaders[i] << endl;

	for (unsigned int i = 0; i < importer.cameraHeaders.size(); i++)
		out << importer.cameraHeaders[i] << endl;

	for (unsigned int i = 0; i < importer.materials.size(); i++)
		out << importer.materials[i] << endl;

	for (unsigned int i = 0; i < importer.transforms.size(); i++)
		out << importer.transforms[i] << endl;

	for (unsigned int i = 0; i < importer.cameras.size(); i++)
		out << importer.cameras[i] << endl;

	return 0;
}