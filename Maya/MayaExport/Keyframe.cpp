#include "Keyframe.h"

using namespace std;

MStatus Keyframe::exportKeyframes(MObject& mayaObject, Keyframes& keyframeCurve, map<const char*, int>& transMap, map<const char*, int>& jointMap, vector<vector<KeyframePoint>> &points, vector<const char*> &curveName, vector<const char*> &attachToName)
{
	MS status;

	MFnAnimCurve curveObject(mayaObject);

	if (!curveObject.isStatic())
	{
		unsigned int keyCount = curveObject.numKeys();

		if (keyCount == 0)
		{
			return MS::kFailure;
		}

		keyframeCurve.numberOfKeyframes = keyCount;

		KeyframePoint point;

		//keyframeCurve.points = new KeyframePoint[keyframeCurve.numberOfKeyframes];

		//keyframeCurve.curveName = curveObject.name().asChar();
		curveName.push_back(curveObject.name().asChar());
		keyframeCurve.curveNameLength = curveObject.name().length();

		if (curveObject.postInfinityType() == MFnAnimCurve::InfinityType::kCycle)
		{
			keyframeCurve.loopAnimation = true;
		}
		else
		{
			keyframeCurve.loopAnimation = false;
		}

		vector<KeyframePoint> temp_point;

		for (unsigned int i = 0; i < keyCount; i++)
		{
			curveObject.getTangent(i, point.tangentInX, point.tangentInY, true);
			curveObject.getTangent(i, point.tangentOutX, point.tangentOutY, false);

			point.inputTangentType = static_cast<KeyframePoint::type>(curveObject.inTangentType(i));
			point.outputTangentType = static_cast<KeyframePoint::type>(curveObject.outTangentType(i));

			point.time = curveObject.time(i).as(MTime::kSeconds);
			point.value = curveObject.value(i);

			//keyframeCurve.points[i] = point;
			temp_point.push_back(point);
		}

		points.push_back(temp_point);

		MPlug outputPlug = curveObject.findPlug("output", true, &status);

		MPlugArray allConnections;
		outputPlug.connectedTo(allConnections, asDst, asSrc);

		// I believe we only care about the one connection. Needs to be checked with blend-shapes.
		MObject controlledObject = allConnections[0].node();
		keyframeCurve.attachToNameLength = allConnections[0].partialName().length();

		//keyframeCurve.attachToName = new char[keyframeCurve.attachToNameLength + 1];
		char* attachToName_temp = new char[keyframeCurve.attachToNameLength + 1];

		for (unsigned int i = 0; i < keyframeCurve.attachToNameLength; i++)
		{ 
			//keyframeCurve.attachToName[i] = allConnections[0].partialName().asChar()[i];
			attachToName_temp[i] = allConnections[0].partialName().asChar()[i];
		}
		//keyframeCurve.attachToName[keyframeCurve.attachToNameLength] = '\0';
		attachToName_temp[keyframeCurve.attachToNameLength] = '\0';

		attachToName.push_back((const char*)attachToName_temp);

		MFnDependencyNode controlledNode(controlledObject);

		if (controlledObject.apiType() == MFn::Type::kJoint)
		{
			keyframeCurve.affectedObjectType = Keyframes::kJoint;
			keyframeCurve.affectedObjectIndex = jointMap[controlledNode.name().asChar()];
		}
		else if (controlledObject.apiType() == MFn::Type::kTransform)
		{
			keyframeCurve.affectedObjectType = Keyframes::kTransform;
			keyframeCurve.affectedObjectIndex = transMap[controlledNode.name().asChar()];
		}
		else if (controlledObject.apiType() == MFn::Type::kBlendShape)
		{
			keyframeCurve.affectedObjectType = Keyframes::kBlendShape;
		}
		else
		{
			keyframeCurve.affectedObjectType = Keyframes::kOther;
		}
	}
	else
	{
		return MS::kFailure;
	}

	return MS::kSuccess;
}