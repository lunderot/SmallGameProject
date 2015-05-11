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

}
