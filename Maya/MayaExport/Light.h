#ifndef LIGHT_H
#define LIGHT_H

#include "maya_includes.h"
#define MAYA_EXPORT
#include "CommonDeclaration.h"
//#include <fstream>
//
//using namespace std;

//klassen tar ett ljud i taget
class exportLight
{
public:

	MStatus exportLightType(MObject& mayaLight, Light& oneLight, vector<const char*>& name);

};




#endif