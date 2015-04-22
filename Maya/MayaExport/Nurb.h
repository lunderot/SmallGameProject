#ifndef NURB_H
#define NURB_H

#include "maya_includes.h"
#include "CommonDeclaration.h"
#include <map>

class exportNurb
{
public:
	MStatus exportNurbSphere(MFnNurbsSurface& mayaNurbSphere, Nurb& nurbSphere, map<const char*, int> transformHierarchy);

};


#endif