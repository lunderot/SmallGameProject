#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "maya_includes.h"
#include "CommonDeclaration.h"

#include <map>

class TransformClass
{
public:
	MStatus exportTransform(MFnTransform& mayaTransform, std::map<const char*, int>& heiraki, unsigned int transformCount, TransformData& transform, vector<const char*>& names);
private:
	MStatus exportName(MFnTransform& mayaTransform, vector<const char*>& names);
	MStatus exportTranslation(MFnTransform& mayaTransform, TransformData& transform);
	MStatus exportRotation(MFnTransform& mayaTransform, TransformData& transform);
	MStatus exportScale(MFnTransform& mayaTransform, TransformData& transform);
};

#endif