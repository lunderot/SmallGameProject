#include "maya_includes.h"
#include <maya/MFnPlugin.h>
#include <iostream>
#include "exporter.h"

using namespace std;

EXPORT MStatus initializePlugin(MObject obj)
{
	MStatus res = MS::kSuccess;

	MFnPlugin myPlugin(obj, "Maya plugin", "1.0", "Any", &res);
	if (MFAIL(res))
	{
		CHECK_MSTATUS(res);
	}
	res = myPlugin.registerCommand("exportToBin", Exporter::creator);

	MGlobal::displayInfo("Maya plugin loaded!");

	return res;
}

EXPORT MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);

	MStatus res = plugin.deregisterCommand("exportToBin");
	MGlobal::displayInfo("Maya plugin unloaded!");

	return MS::kSuccess;
}