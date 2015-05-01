#include "Light.h"
#include "maya/mPlug.h"

//MStatus exportLight::exportLightType(MFnLight& mayaLight, LightHeader& Lheader, Light& oneLight)
MStatus exportLight::exportLightType(MObject& inputLight, Light& oneLight, vector<const char*>& name)
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
		oneLight.type = Light::light_type::ePoint;
		oneLight.dType = (Light::decay_type)mayaLight.decayRate(&status);
	}

	
	else if (inputLight.hasFn(MFn::Type::kDirectionalLight))
	{
		oneLight.type = Light::light_type::eDirectional;
		oneLight.dType = Light::eNone;
	}


	else if (inputLight.hasFn(MFn::Type::kSpotLight))
	{
		oneLight.type = Light::light_type::eSpot;
		oneLight.dType = (Light::decay_type)mayaLight.decayRate(&status);
	}


	else if (inputLight.hasFn(MFn::Type::kAreaLight))
	{
		oneLight.type = Light::light_type::eArea;
		oneLight.dType = (Light::decay_type)mayaLight.decayRate(&status);
	}


	else if (inputLight.hasFn(MFn::Type::kVolumeLight))
	{
		oneLight.type = Light::light_type::eVolume;
		oneLight.dType = Light::eNone;
	}
	else
		return MS::kFailure;


	name.push_back(mayaLight.name().asChar());
	oneLight.name_Length = mayaLight.name().length();

	MGlobal::displayInfo("Maya Light Type: " + MString() + mayaLight.typeName());
	MGlobal::displayInfo("Maya Decay Type: " + MString() + (Light::decay_type)mayaLight.decayRate(&status));
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



	return MS::kSuccess;

}