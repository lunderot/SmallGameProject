#ifndef EXPORTER_H
#define EXPORTER_H

#include "maya_includes.h"

class Exporter : public MPxCommand {
public:
	Exporter() {};
	virtual MStatus doIt(const MArgList& argList);
	static void* creator();
	static MSyntax newSyntax();
private:
	std::string outputDir = "";
	std::string fileName = "";
};

#endif