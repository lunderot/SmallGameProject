#ifndef WRITETOFILE_H
#define WRITETOFILE_H

#include "maya_includes.h"
#include "CommonDeclaration.h"
#include <fstream>
#include <string>

using namespace std;

class WriteToFile
{
public:
	~WriteToFile();
	bool binaryFilePath(string filePathAndFileName);
	bool ASCIIFilePath(string filePathAndFileName);

	template<typename dataType>
	void writeToFiles(dataType* toWrite, unsigned int numOfElementToWrite = 1, unsigned int startIndex = 0)
	{
		MGlobal::displayInfo("WriteToFile::writeToFiles()");

		if (ASCIIFile.is_open())
		{
			MGlobal::displayInfo("Writhing to ASCIIfile");
			for (unsigned int i = startIndex; i < numOfElementToWrite + startIndex; i++)
				ASCIIFile << toWrite[i] << endl;
		}

		if (binFile.is_open())
		{
			MGlobal::displayInfo("Writhing to binaryfile");
			char* output = (char*)toWrite;
			binFile.write(output, sizeof(dataType) * (numOfElementToWrite - startIndex));
		}

	}

	void writeToFiles(char* toWrite, unsigned int numOfElementToWrite = 1, unsigned int startIndex = 0)
	{
		MGlobal::displayInfo("WriteToFile::writeToFiles()");

		if (ASCIIFile.is_open())
		{
			MGlobal::displayInfo("Writhing to ASCIIfile");
			for (unsigned int i = startIndex; i < numOfElementToWrite + startIndex; i++)
				ASCIIFile << toWrite[i] << endl;
		}

		if (binFile.is_open())
		{
			MGlobal::displayInfo("Writhing to binaryfile");
			std::string tmp;
			for (unsigned int i = startIndex; i < numOfElementToWrite + startIndex; i++)
			{
				tmp = toWrite[0];
				binFile.write((char*)&tmp, tmp.size());
			}
		}

	}

	void OpenFiles();
	void CloseFiles();

private:
	string binFileName;
	string ASCIIFileName;
	ofstream binFile;
	ofstream ASCIIFile;
};



#endif