#ifndef LIGHT_H
#define LIGHT_H

#include "maya_includes.h"
#include "CommonDeclaration.h"
//#include <fstream>
//
//using namespace std;

//klassen tar ett ljud i taget
class exportLight
{
public:

	MStatus exportLightType(MFnLight& mayaLight, LightHeader& Lheader, Light& oneLight);

};




#endif