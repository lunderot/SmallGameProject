#include "Light.h"

void Light::ExportLights(FbxScene* scene, std::string fileName)
{
	// From the Importer
	Importer importer;
	if (!importer.importFile(fileName))
	{
		FBXSDK_printf("Failed to Import the file");
	}
	// Get root node
	FbxNode* rootNode = scene->GetRootNode();

	const ImporterLight* importedLights = importer.getLight();

	for (unsigned int i = 0; i < importer.getNumLights(); i++)
	{
		FbxLight* light = FbxLight::Create(scene, importedLights[i].name);
		FbxNode* lightNode = FbxNode::Create(scene, "");

		// Light type
		if (importedLights[i].type == 0)
			light->LightType.Set(FbxLight::EType::ePoint);
		else if (importedLights[i].type == 1)
			light->LightType.Set(FbxLight::EType::eDirectional);
		else if (importedLights[i].type == 2)
			light->LightType.Set(FbxLight::EType::eSpot);
		else if (importedLights[i].type == 3)
			light->LightType.Set(FbxLight::EType::eArea);
		else
			light->LightType.Set(FbxLight::EType::eVolume);

		//Color & Intensity
		light->Color.Set(FbxDouble3(importedLights[i].color[0], importedLights[i].color[1], importedLights[i].color[2]));
		light->Intensity.Set(importedLights[i].intensity * 100);
		cout << importedLights[i].intensity << endl;

		// Decay Type
		if (importedLights[i].dType == 0)
			light->DecayType.Set(FbxLight::EDecayType::eNone);
		else if (importedLights[i].dType == 1)
			light->DecayType.Set(FbxLight::EDecayType::eLinear);
		else if (importedLights[i].dType == 2)
			light->DecayType.Set(FbxLight::EDecayType::eQuadratic);
		else
			light->DecayType.Set(FbxLight::EDecayType::eCubic);

		light->CastShadows.Set(importedLights[i].cast_shadows);
		light->ShadowColor.Set(FbxDouble3(importedLights[i].shadow_color[0], importedLights[i].shadow_color[1], importedLights[i].shadow_color[2]));
		
		for (int j = 0; j < importedLights[i].numberOfParents; j++)
		{
			int parentID = importedLights[i].parentID[j] + 1;

			FbxNode* parentNode = scene->GetNode(parentID);
			parentNode->SetNodeAttribute(light);
		}

	}
}