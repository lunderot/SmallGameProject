#include "Transform.h"

MStatus TransformClass::exportTransform(MFnTransform& mayaTransform, std::map<const char*, int>& heiraki, unsigned int transformCount, TransformHeader& transformHeader, Transform& transform)
{	
	MGlobal::displayInfo("TransformClass::exportTransform()");
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


	transformHeader.name_Length = mayaTransform.name(&status).numChars();

	if (status == MS::kSuccess)
		status = exportName(mayaTransform, transform);
	if (status == MS::kSuccess)
		status = exportTranslation(mayaTransform, transform);
	if (!status == MS::kSuccess)
		status = exportRotation(mayaTransform, transform);
	if (!status == MS::kSuccess)
		status = exportScale(mayaTransform, transform);

	return status;
}

MStatus TransformClass::exportName(MFnTransform& mayaTransform, Transform& transform)
{
	MGlobal::displayInfo("TransformClass::exportName()");
	MS status;
	transform.name = mayaTransform.name(&status).asChar();
	MGlobal::displayInfo(mayaTransform.name());
	return status;
}

MStatus TransformClass::exportTranslation(MFnTransform& mayaTransform, Transform& transform)
{
	MGlobal::displayInfo("TransformClass::exportTranslation()");
	MS status;
	MSpace space;
	MVector pos = mayaTransform.getTranslation(space.kTransform, &status);
	pos.get(transform.position);
	return status;
}

MStatus TransformClass::exportRotation(MFnTransform& mayaTransform, Transform& transform)
{
	MGlobal::displayInfo("TransformClass::exportRotation()");
	return mayaTransform.getRotationQuaternion(transform.rotation[0], transform.rotation[1], transform.rotation[2], transform.rotation[3]);
}

MStatus TransformClass::exportScale(MFnTransform& mayaTransform, Transform& transform)
{
	MGlobal::displayInfo("TransformClass::exportScale()");
	return mayaTransform.getScale(transform.scale);
}