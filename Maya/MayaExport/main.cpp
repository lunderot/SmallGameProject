#include "maya_includes.h"
#include <maya/MFnPlugin.h>
#include <iostream>
#include "exporter.h"

using namespace std;

class MyCommand : public MPxCommand
{
public:
	MyCommand() {};
	virtual MStatus doIt(const MArgList& argList);
	static void* creator();
protected:
private:
};

void* MyCommand::creator()
{
	return new MyCommand; 
}

MStatus MyCommand::doIt(const MArgList& argList)
{
	MGlobal::displayInfo("Hello World!");
	return MS::kSuccess;
}

EXPORT MStatus initializePlugin(MObject obj)
{
	MStatus res = MS::kSuccess;

	MFnPlugin myPlugin(obj, "Maya plugin", "1.0", "Any", &res);
	if (MFAIL(res))
	{
		CHECK_MSTATUS(res);
	}
	res = myPlugin.registerCommand("myCommand", Exporter::creator);

	MGlobal::displayInfo("Maya plugin loaded!");

	return res;
}

EXPORT MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);

	MStatus res = plugin.deregisterCommand("myCommand");
	MGlobal::displayInfo("Maya plugin unloaded!");

	return MS::kSuccess;
}