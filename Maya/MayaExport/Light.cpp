#include "Light.h"
#include "maya/mPlug.h"

//MStatus exportLight::exportLightType(MFnLight& mayaLight, LightHeader& Lheader, Light& oneLight)
MStatus exportLight::exportLightType(MObject& inputLight, LightData& oneLight, vector<const char*>& name, map<const char*, int> transformHierarchy, vector<vector <int> >& parentID)
{


	//MGlobal::displayInfo(MString() + camera.name);

	//------------------------------------------------------//
	//                LIGHT TYPE                            //
	//                DECAY TYPE                            //
	//------------------------------------------------------//

	MFnNonAmbientLight mayaLight(inputLight);

	MStatus status;

	if (inputLight.hasFn(MFn::Type::kPointLight))
	{	
		oneLight.type = light_type::ePoint;
		oneLight.dType = (decay_type)mayaLight.decayRate(&status);
	}

	
	else if (inputLight.hasFn(MFn::Type::kDirectionalLight))
	{
		oneLight.type = light_type::eDirectional;
		oneLight.dType = decay_type::eNone;
	}


	else if (inputLight.hasFn(MFn::Type::kSpotLight))
	{
		oneLight.type = light_type::eSpot;
		oneLight.dType = (decay_type)mayaLight.decayRate(&status);
	}


	else if (inputLight.hasFn(MFn::Type::kAreaLight))
	{
		oneLight.type = light_type::eArea;
		oneLight.dType = (decay_type)mayaLight.decayRate(&status);
	}


	else if (inputLight.hasFn(MFn::Type::kVolumeLight))
	{
		oneLight.type = light_type::eVolume;
		oneLight.dType = decay_type::eNone;
	}
	else
		return MS::kFailure;


	name.push_back(mayaLight.name().asChar());
	oneLight.name_Length = mayaLight.name().length();

	MGlobal::displayInfo("Maya Light Type: " + MString() + mayaLight.typeName());
	MGlobal::displayInfo("Maya Decay Type: " + MString() + (decay_type)mayaLight.decayRate(&status));
	MGlobal::displayInfo("Light Type: " + MString() + oneLight.type);
	MGlobal::displayInfo("Decay Type: " + MString() + oneLight.dType);


	// ----------------------------------------------------//
	//                    COLOR                            //
	// ----------------------------------------------------//
	
	MColor colorOfLight = mayaLight.color();
	
	oneLight.color[0] = colorOfLight.r;
	oneLight.color[1] = colorOfLight.g;
	oneLight.color[2] = colorOfLight.b;
	

	MGlobal::displayInfo("OneLight Color: " + MString() + oneLight.color[0] + " " + MString() + oneLight.color[1] + " " + MString() + oneLight.color[2]);
	MGlobal::displayInfo("Maya Color: " + MString() + colorOfLight.r + " " + MString() + colorOfLight.g + " " + MString() + colorOfLight.b);
	

	// ----------------------------------------------------//
	//               INTENSITY                             //
	// ----------------------------------------------------//

	oneLight.intensity = mayaLight.intensity();

	MGlobal::displayInfo("OneLight Intensity: " + MString() + oneLight.intensity);
	MGlobal::displayInfo("Maya Intensity: " + MString() + mayaLight.intensity());

	// ----------------------------------------------------//
	//               CAST SHADOW                           //
	// ----------------------------------------------------//

	//MFnNonAmbientLight fast i objektform
	MObject reachCastSHadowFunc = mayaLight.object();

	MFnDependencyNode aDNode(reachCastSHadowFunc);
	MPlug aPlug =  aDNode.findPlug("useDepthMapShadows");

	aPlug.getValue(oneLight.cast_shadows);

	MGlobal::displayInfo("OneLight Cast shadow?: " + MString() + oneLight.cast_shadows);

	// ----------------------------------------------------//
	//               SHADOW COLOR                          //
	// ----------------------------------------------------//

	MColor colorOfShadow = mayaLight.shadowColor();

	// hänmtar RGB 0 - 1 (även om det är inställd på HSV eller RGB 0 -255)
	oneLight.shadow_color[0] = colorOfShadow.r;
	oneLight.shadow_color[1] = colorOfShadow.g;
	oneLight.shadow_color[2] = colorOfShadow.b;

	MGlobal::displayInfo("OneLight Shadow Color: " + MString() + oneLight.shadow_color[0] + " " + MString() + oneLight.shadow_color[1] + " " + MString() + oneLight.shadow_color[2]);
	MGlobal::displayInfo("Maya Shadow Color: " + MString() + colorOfShadow.r + " " + MString() + colorOfShadow.g + " " + MString() + colorOfShadow.b);


	// ---------------------------------------------------//
	//					PARENT ID                         //
	// ---------------------------------------------------//

	MFnLight mayaLIGHTO(inputLight);


	vector<int> parentIndex(mayaLight.parentCount());

	for (unsigned int i = 0; i < mayaLight.parentCount(); i++)
	{
		MObject parent = mayaLight.parent(i);
		MFnDagNode storeParent(parent);
		parentIndex[i] = transformHierarchy[storeParent.name().asChar()];
	}

	oneLight.numberOfParent = mayaLight.parentCount();
	parentID.push_back(parentIndex);


	return MS::kSuccess;

}