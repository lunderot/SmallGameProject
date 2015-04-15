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

	template <typename dataType, typename describerType>
	//Used for our different struct with pointers
	void writeToFiles(dataType* toWrite, describerType* numOfElemetsPointingTo, unsigned int numOfElementToWrite = 1, unsigned int startIndex = 0)
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
			for (unsigned int i = startIndex; i < numOfElementToWrite + startIndex; i++)
				toWrite[i].WriteBinary(&numOfElemetsPointingTo[i], binFile);
		}
	}

	template <typename dataType>
	//Used for non-pointers
	void writeToFiles(dataType* toWrite, unsigned int numOfElementToWrite = 1, unsigned int startIndex = 0)
	{
		MGlobal::displayInfo("WriteToFile::writeToFiles()");
		if (binFile.is_open())
		{
			MGlobal::displayInfo("Writhing to binaryfile");
			for (unsigned int i = startIndex; i < numOfElementToWrite + startIndex; i++)
				binFile.write((const char*)&toWrite[i], sizeof(dataType));
		}

		if (ASCIIFile.is_open())
		{
			MGlobal::displayInfo("Writhing to ASCIIfile");
			for (unsigned int i = startIndex; i < numOfElementToWrite + startIndex; i++)
				ASCIIFile << toWrite[i] << endl;
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