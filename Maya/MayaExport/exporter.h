#ifndef EXPORTER_H
#define EXPORTER_H

#include "maya_includes.h"

using namespace std;

class Exporter : public MPxCommand {
public:
	Exporter() {};
	virtual MStatus doIt(const MArgList& argList);
	static void* creator();
};

#endif