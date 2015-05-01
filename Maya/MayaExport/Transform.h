#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "maya_includes.h"
#include "CommonDeclaration.h"

#include <map>

class TransformClass
{
public:
	MStatus exportTransform(MFnTransform& mayaTransform, std::map<const char*, int>& heiraki, unsigned int transformCount, Transform& transform, vector<const char*>& names);
private:
	MStatus exportName(MFnTransform& mayaTransform, vector<const char*>& names);
	MStatus exportTranslation(MFnTransform& mayaTransform, Transform& transform);
	MStatus exportRotation(MFnTransform& mayaTransform, Transform& transform);
	MStatus exportScale(MFnTransform& mayaTransform, Transform& transform);
};

#endif