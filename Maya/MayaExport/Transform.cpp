#include "Transform.h"

MStatus TransformClass::exportTransform(MFnTransform& mayaTransform, std::map<const char*, int>& heiraki, unsigned int transformCount, TransformData& transform, vector<const char*>& names)
{	
	MStatus status = MS::kSuccess;

	heiraki[mayaTransform.name().asChar()] = transformCount;

	MGlobal::displayInfo(MString() + mayaTransform.parentCount());
	MFnDependencyNode parent(mayaTransform.parent(0));
	MGlobal::displayInfo(parent.name());
	if (parent.name() == "world")
	{
		transform.parentID = -1;
	}
	else
	{
		transform.parentID = heiraki[parent.name().asChar()];
	}

	transform.name_Length = mayaTransform.name(&status).numChars();
	if (status != MS::kSuccess)
	{
		MGlobal::displayInfo("Failure at TransformClass::exportTransform() with 'mayaTransform.name(&status).numChars()'" );
		return status;
	}

	status = exportName(mayaTransform, names);
	if (status != MS::kSuccess)
	{
		MGlobal::displayInfo("Failure at TransformClass::exportName()");
		return status;
	}

	status = exportTranslation(mayaTransform, transform);
	if (status != MS::kSuccess)
	{
		MGlobal::displayInfo("Failure at TransformClass::exportTranslation()");
		return status;
	}

	status = exportRotation(mayaTransform, transform);
	if (status != MS::kSuccess)
	{
		MGlobal::displayInfo("Failure at TransformClass::exportRotation()");
		return status;
	}

	status = exportScale(mayaTransform, transform);
	if (status != MS::kSuccess)
	{
		MGlobal::displayInfo("Failure at TransformClass::exportScale()");
	}

	return status;
}

MStatus TransformClass::exportName(MFnTransform& mayaTransform, vector<const char*>& names)
{
	MS status;
	const char* name = mayaTransform.name(&status).asChar();
	names.push_back(name);
	MGlobal::displayInfo(mayaTransform.name());
	return status;
}

MStatus TransformClass::exportTranslation(MFnTransform& mayaTransform, TransformData& transform)
{
	MS status;
	MSpace space;
	MVector pos = mayaTransform.getTranslation(space.kTransform, &status);
	pos.get(transform.position);
	return status;
}

MStatus TransformClass::exportRotation(MFnTransform& mayaTransform, TransformData& transform)
{
	return mayaTransform.getRotationQuaternion(transform.rotation[0], transform.rotation[1], transform.rotation[2], transform.rotation[3]);
}

MStatus TransformClass::exportScale(MFnTransform& mayaTransform, TransformData& transform)
{

	return mayaTransform.getScale(transform.scale);
}