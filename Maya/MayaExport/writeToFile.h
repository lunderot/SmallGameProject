#ifndef WRITETOFILE_H
#define WRITETOFILE_H

#include "maya_includes.h"
#define MAYA_EXPORT
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

	template <typename dataType>
	//Used for our different struct with pointers
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
			for (unsigned int i = startIndex; i < numOfElementToWrite + startIndex; i++)
				toWrite[i].WriteBinary(binFile);
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