#ifndef NURB_H
#define NURB_H

#include "maya_includes.h"
#define MAYA_EXPORT
#include "CommonDeclaration.h"
#include <map>

class exportNurb
{
public:
	MStatus exportNurbSphere(MFnNurbsSurface& mayaNurbSphere, Nurb& nurbSphere, map<const char*, int> transformHierarchy, vector<const char*>& name, vector<vector <int> >& parentID);

};


#endif