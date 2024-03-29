#ifndef JOINT_H
#define JOINT_H

#include "maya_includes.h"
#include <vector>
#include <map>
#include "CommonDeclaration.h"

using namespace std;

class JointExporter
{
public:
	MStatus exportJoint(MFnIkJoint& mayaJoint, std::map<const char*, int>& jointHeiraki, std::map<const char*, int>& TransformHeiraki, unsigned int jointCount, JointData& joint, vector<const char*>& names );
	
private:
	MStatus exportJointOrientation(MFnIkJoint& mayaJoint, JointData& joint);
};

#endif