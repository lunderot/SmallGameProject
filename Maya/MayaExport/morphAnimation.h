#ifndef MORPHANIMATION_H
#define MORPHANIMATION_H
#define MAYA_EXPORT
#include "CommonDeclaration.h"
#include "maya_includes.h"
#include <map>

class MorphAnimations
{
public:
	MStatus exportMorphAnimation(MItDependencyNodes &it, MorphAnimationData& morphAnim, vector<vector<double>>& positions, vector <const char*>& blendShapeName, map<const char*, unsigned int> meshMap);
};
#endif