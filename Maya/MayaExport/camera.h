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
	MStatus exportCamera(MFnCamera& mayaCamera, cameraData& camera, map<const char*, int>& transformHeiraki, vector<const char*>& names, vector<vector<unsigned int>>& parentIDs);

};
#endif