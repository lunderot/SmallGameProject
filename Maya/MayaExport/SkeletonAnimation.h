#ifndef SKELETONANIMATION_H
#define SKELETONANIMATION_H

#include "maya_includes.h"
#define MAYA_EXPORT
#include "CommonDeclaration.h"
#include "maya/MFnSkinCluster.h"
#include "maya/MDagPathArray.h"
#include "maya/MItGeometry.h"

#include <map>
#include <algorithm>

class SkeletonAnimation
{
public:
	MStatus exportSkin(MObject& mayaObject, SkinAnimation& skin, map<const char*, int>& jointMap, map<const char*, unsigned int>& meshMap);
protected:
private:
	bool asDst = false, asSrc = true;
};

#endif //SKELETONANIMATION_H