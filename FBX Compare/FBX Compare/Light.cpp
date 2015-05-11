#include "Light.h"

Light::Light()
{

}

Light::~Light()
{

}

bool Light::GetInfo(FbxNode* pNode, LightInfo& pLightInfo)
{
	FbxLight* lLight = pNode->GetLight();

	if (lLight)
	{
		FBXSDK_printf("current light node: %s\n", pNode->GetName());

		this->GetStats(lLight, pLightInfo);

		return true;
	}
	return false;
}

void Light::GetStats(FbxLight* pLight, LightInfo& pLightInfo)
{
	pLightInfo.cast_shadows = pLight->CastShadows.Get();
	pLightInfo.color = pLight->Color.Get();
	pLightInfo.decay_type = pLight->DecayType.Get();
	pLightInfo.intensity = pLight->Intensity.Get();
	pLightInfo.light_type = pLight->LightType.Get();
	pLightInfo.shadow_color = pLight->ShadowColor.Get();
}
