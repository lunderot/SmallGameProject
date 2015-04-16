#include "morphAnimation.h"
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MPlug.h>
#include <maya/MItGeometry.h>

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

	MObjectArray baseObjects;

	MGlobal::displayInfo(MString() + "Blend Shape: " + fn.name().asChar());
	morphAnim.blendShapeName = fn.name().asChar();
	morphHeader.blendShape_name_length = fn.name().length();
	fn.getBaseObjects(baseObjects);

	for (unsigned int i = 0; i < baseObjects.length(); i++)
	{
		MObject base = baseObjects[i];
		MFnDependencyNode baseDep(base);

		//morphAnim.baseName = baseDep.name().asChar();
		//morphHeader.base_name_length = baseDep.name().length();

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

			for (unsigned int k = 0; k < targets.length(); k++)
			{
				MItGeometry targetGeo(targets[k]);
				
				morphHeader.nrOfVertsPerMesh = targetGeo.count();
				tmpCount += targetGeo.count();
				morphAnim.position.resize(tmpCount);

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
