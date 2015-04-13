#include "Importer.h"
#include <fstream>
#include <iostream>
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
	cout << fileByteSize << endl;

	fileData = new char[fileByteSize];
	in.read(fileData, fileByteSize);
	in.close();
	cout << fileData << endl;

	unsigned int offset = 0;

	Header extractedHeader;
	memcpy(&extractedHeader, fileData, 0);
	offset += sizeof(Header);
	headers.push_back(extractedHeader);
	cout << headers[0] << endl;
	cout << extractedHeader << endl;

	MaterialHeader extractedMaterialHeader;
	for (unsigned int i = 0; i < extractedHeader.material_count; i++)
	{
		memcpy(&extractedMaterialHeader, &fileData[offset], 0);
		offset += sizeof(MaterialHeader);
		materialHeaders.push_back(&extractedMaterialHeader);
	}



	return true;
};