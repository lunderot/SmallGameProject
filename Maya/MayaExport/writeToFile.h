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

	template <typename dataType>
	void writeToFiles(dataType* toWrite, unsigned int numberOfElementToWrite, unsigned int startIndex = 0)
	{	
		MGlobal::displayInfo("WriteToFile::writeToFiles()");
		if (binFile.is_open())
		{
			MGlobal::displayInfo("Writhing to binaryfile");
			binFile.write((const char*)&toWrite[startIndex], sizeof(dataType) * numberOfElementToWrite);
		}

		if (ASCIIFile.is_open())
		{
			MGlobal::displayInfo("Writhing to ASCIIfile");
			for (unsigned int i = startIndex; i < numberOfElementToWrite + startIndex; i++)
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