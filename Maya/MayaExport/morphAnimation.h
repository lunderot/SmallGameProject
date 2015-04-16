#ifndef MORPHANIMATION_H
#define MORPHANIMATION_H

#include "CommonDeclaration.h"
#include "maya_includes.h"
#include <map>

class MorphAnimations
{
public:
	MStatus exportMorphAnimation(MItDependencyNodes &it, morphAnimationHeader &morphHeader, MorphAnimation &morphAnim, map<const char*, unsigned int> meshMap);

};
#endif