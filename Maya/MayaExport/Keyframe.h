#ifndef KEYFRAME_H
#define KEYFRAME_H

#include "maya_includes.h"
#define MAYA_EXPORT
#include "CommonDeclaration.h"

#include "maya/MFnAnimCurve.h"
#include "maya/MPlug.h"

#include <map>

class Keyframe
{
public:
	MStatus exportKeyframes(MObject& mayaObject, KeyframesData& keyframeCurve, map<const char*, int>& transMap, map<const char*, int>& jointMap, vector<vector<KeyframePoint>> &points, vector<const char*> &curveName, vector<const char*> &attachToName);
protected:
private:
	bool asDst = false, asSrc = true;
};

#endif // KEYFRAME_H