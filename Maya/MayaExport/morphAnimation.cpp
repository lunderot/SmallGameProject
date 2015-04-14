#include "morphAnimation.h"
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MPlug.h>
#include <maya/MItGeometry.h>

MStatus MorphAnimations::exportMorphAnimation()
{
	cout << "HEJ MAGNUUUUUUUUUZ ;D ;D" << endl;
	MStatus status;
	MItDependencyNodes blendShape(MFn::kBlendShape);

	while (!blendShape.isDone())
	{
		MFnBlendShapeDeformer fn(blendShape.item());

		// disable envelope
		MPlug plug = fn.findPlug("en");

		plug.setValue(0.0f);
		MObjectArray baseObjects;
		cout << "Blend Shape: " << fn.name().asChar() << endl;
		fn.getBaseObjects(baseObjects);

		for (unsigned int i = 0; i < baseObjects.length(); i++)
		{
			MObject base = baseObjects[i];
			MFnDependencyNode baseDep(base);
			cout << "Base: " << baseDep.name().asChar() << endl;

			unsigned int weights = fn.numWeights();
			cout << "Nr of weights: " << weights << endl;

			for (unsigned int j = 0; j < weights; j++)
			{
				MObjectArray targets;

				fn.getTargets(base, j, targets);
				cout << "Nr of targets: " << targets.length() << endl;

				for (unsigned int k = 0; k < targets.length(); k++)
				{
					MItGeometry targetGeo(targets[k]);

					//cout << "Nr of points: " << targetGeo.count << endl;
					//MGlobal::displayInfo(MString() + targetGeo.count);
					while (!targetGeo.isDone())
					{
						MPoint p = targetGeo.position();
						cout << p.x << "/" << p.y << "/" << p.z << endl;
						targetGeo.next();
					}
				}
			}
		}

		// enable envelope
		plug.setValue(1.0f);

		blendShape.next();
	}

	return status;

}
