#include "Light.h"

MStatus exportLight::exportLightType(MFnLight& mayaLight, LightHeader& Lheader, Light& oneLight)
{
	//MGlobal::displayInfo("exportLight::exportLightType()");

	// ____________________________________________________//
	//                LIGHT TYPE     
	//                DECAY TYPE
	// ____________________________________________________//


	MFn::Type mLight = mayaLight.type();
	
	// ej ambient?

	if (mLight == MFn::Type::kPointLight)
		oneLight.type = Light::ePoint;

	
	if (mLight == MFn::Type::kDirectionalLight)
		oneLight.type = Light::eDirectional;
		oneLight.dType = Light::noSuppDecay;

	if (mLight == MFn::Type::kSpotLight)
		oneLight.type = Light::eSpot;

	if (mLight == MFn::Type::kAreaLight)
		oneLight.type = Light::eArea;

	if (mLight == MFn::Type::kVolumeLight)
		oneLight.type = Light::eVolume;
		oneLight.dType = Light::noSuppDecay;

	MGlobal::displayInfo("Light Type: " + mayaLight.typeName());
	MGlobal::displayInfo("Decay Type: " + MString() + oneLight.dType);

	// --------------- COLOR -------------------

	
	MColor colorOfLight = mayaLight.color();
	
	oneLight.color[0] = colorOfLight.r;
	oneLight.color[1] = colorOfLight.g;
	oneLight.color[2] = colorOfLight.b;
	


	MGlobal::displayInfo("Color: " + MString() + colorOfLight.r + " " + MString() + colorOfLight.g + " " + MString() + colorOfLight.b);
	

	// ------------ INTENSITY ------------

	oneLight.intensity = mayaLight.intensity();

	MGlobal::displayInfo("Intensity: " + MString() + mayaLight.intensity());




	return MS::kSuccess;

}