#ifndef CAMERA_H
#define CAMERA_H
#include "maya_includes.h"
#include "CommonDeclaration.h"
#include "Transform.h"
#include <fstream>
#include <map>

using namespace std;

class Camera
{
public:
	MStatus exportCamera(MFnCamera& mayaCamera, camera& camera, CameraHeader& camHeader, map<const char*, int>& transformHeiraki);

};
#endif