#include "morphAnimation.h"
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MPlug.h>
#include <maya/MItGeometry.h>
#include <maya/MAnimControl.h>
#include <maya/MItKeyframe.h>

MStatus MorphAnimations::exportMorphAnimation(MItDependencyNodes &it, morphAnimationHeader &morphHeader, MorphAnimation &morphAnim, map<const char*, unsigned int> meshMap)
{
	cout << "HEJ MAGNUUUUUUUUUZ ;D ;D" << endl;
	MStatus status;
	unsigned int counter = 0;
	unsigned int tmpCount = 0;

	//MFnBlendShapeDeformer fn(blendShape.item());

	// disable envelope
	MFnBlendShapeDeformer fn(it.item());
	
	MPlug plug = fn.findPlug("en");
	plug.setValue(0.0f);

	MPlug shapePlug = fn.findPlug("World Mesh", &status);
	if (status != MS::kSuccess)
		MGlobal::displayInfo(MString() + "FAIL!!!!!!!!!!!!!!!!!!!");

	MObject test = shapePlug.asMObject();
	MFnMesh testMesh(test);
	MGlobal::displayInfo(MString() + "Super duper test mesh name: " + testMesh.name().asChar());

	MObjectArray baseObjects;

	MGlobal::displayInfo(MString() + "Blend Shape: " + fn.name().asChar());
	morphAnim.blendShapeName = fn.name().asChar();
	morphHeader.blendShape_name_length = fn.name().length();
	fn.getBaseObjects(baseObjects);

	for (unsigned int i = 0; i < baseObjects.length(); i++)
	{
		MObject base = baseObjects[i];
		MFnDependencyNode baseDep(base);

		morphAnim.meshID = meshMap[baseDep.name().asChar()];

		MGlobal::displayInfo(MString() + "Base: " + baseDep.name().asChar());

		unsigned int weights = fn.numWeights();

		morphHeader.nrOfWeights = weights;
		MGlobal::displayInfo(MString() + "Nr of Weights: " + weights);

		for (unsigned int j = 0; j < weights; j++)
		{
			MObjectArray targets;

			fn.getTargets(base, j, targets);

			morphHeader.nrOfTargets = targets.length();
			MGlobal::displayInfo(MString() + "Nr of targets: " + targets.length());

			MFnDependencyNode targetDep(targets[j]);
			MGlobal::displayInfo(MString() + "Target name: " + targetDep.name().asChar());


			for (unsigned int k = 0; k < targets.length(); k++)
			{
				MItGeometry targetGeo(targets[k]);
				
				morphHeader.nrOfVertsPerMesh = targetGeo.count();
				tmpCount += targetGeo.count();
				morphAnim.position.resize(tmpCount);

				MTime start = MAnimControl::animationStartTime();
				MTime end = MAnimControl::animationEndTime();
			

				MGlobal::displayInfo(MString() + "Start: " + start.as(MTime::kPALFrame));
				MGlobal::displayInfo(MString() + "End: " + end.as(MTime::kPALFrame));
				int startFrame = start.value();
				int endFrame = end.value();

				bool gotAnimData = MAnimUtil::isAnimated(baseObjects[i], false, &status);

				if (gotAnimData == false)
					MGlobal::displayInfo(MString() + "FAIL WITH KEYFRAMES");
				else
				{
					MGlobal::displayInfo(MString() + "GREAT SUCCESS WITH KEYFRAMES");
					
					MFnAnimCurve anim(base, &status);

					MGlobal::displayInfo(MString() + "NR OF KEYS:" + anim.numKeyframes());

					for (int p = startFrame; p < endFrame; p++)
					{
						MAnimControl::setCurrentTime(MTime(p, MTime::kPALFrame));
						MTime asd = anim.time(p, &status);
						MGlobal::displayInfo(MString() + "THE TIME!!!! "+ asd.as(MTime::kPALFrame));

						for (unsigned int w = 0; w < fn.numWeights(); w++)
						{
							MGlobal::displayInfo(MString() + "Current Weight: " + fn.weight(w));
							MGlobal::displayInfo(MString() + p);
						}
					}
				}

				//MGlobal::displayInfo(MString() + targetGeo.count);
				while (!targetGeo.isDone())
				{
					MPoint p = targetGeo.position();
					morphAnim.position[counter].push_back(p.x);
					morphAnim.position[counter].push_back(p.y);
					morphAnim.position[counter].push_back(p.z);
					MGlobal::displayInfo(MString() + "Position: " + p.x + "/" + p.y + "/" + p.z);
					counter++;
					targetGeo.next();
				}
			}
		}

		// enable envelope
		plug.setValue(1.0f);

	}

	return status;

}
