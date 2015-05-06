#include "JointExporter.h"
#include "Transform.h"
MStatus JointExporter::exportJoint(MFnIkJoint& mayaJoint, std::map<const char*, int>& jointHeiraki, std::map<const char*, int>& transformHeiraki, unsigned int jointCount, JointData& joint, vector<const char*>& names)
{
	MS status;

	TransformClass transformExport;
	status = transformExport.exportTransform(mayaJoint, jointHeiraki, jointCount, joint.transform, names);
	if (status != MS::kSuccess)
	{
		MGlobal::displayInfo("Failure at TransformClass::exportTransform()");
		return status;
	}

	status = exportJointOrientation(mayaJoint, joint);
	if (status != MS::kSuccess)
	{
		MGlobal::displayInfo("Failure at JointExporter::exportJointOrientation()");
	}

	return status;
}

MStatus JointExporter::exportJointOrientation(MFnIkJoint& mayaJoint, JointData& joint)
{
	MS status = MS::kSuccess;
	MQuaternion orientation;
	status = mayaJoint.getOrientation(orientation);
	if (status != MS::kSuccess)
		return status;

	orientation.get(joint.jointOrientation);
	return status;
}