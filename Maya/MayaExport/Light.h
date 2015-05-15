#ifndef LIGHT_H
#define LIGHT_H

#include "maya_includes.h"
#define MAYA_EXPORT
#include "CommonDeclaration.h"
#include <map>
//#include <fstream>
//
//using namespace std;

//klassen tar ett ljud i taget
class exportLight
{
public:

	MStatus exportLightType(MObject& mayaLight, LightData& oneLight, vector<const char*>& name, map<const char*, int> transformHierarchy, vector<vector <int> >& parentID);

};




#endif