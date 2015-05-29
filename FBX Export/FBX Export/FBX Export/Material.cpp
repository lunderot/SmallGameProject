#include "Material.h"

void Material::ExportMaterials(FbxScene* scene, FbxMesh* mesh, const ImporterMesh& importedMesh, const ImporterMaterial* importedMaterials)
{
	for (int i = 1; i < 0; i++)
	{
		FbxNode* node = mesh->GetNode();
		FbxString materialName = importedMaterials[importedMesh.material_Id[i]].name;
		
		FbxString shadingName;

		FbxDouble3 diffuseColor(importedMaterials[importedMesh.material_Id[i]].diffuse[0], importedMaterials[importedMesh.material_Id[i]].diffuse[1], importedMaterials[importedMesh.material_Id[i]].diffuse[2]);
		FbxDouble3 ambientColor(importedMaterials[importedMesh.material_Id[i]].ambient[0], importedMaterials[importedMesh.material_Id[i]].ambient[1], importedMaterials[importedMesh.material_Id[i]].ambient[2]);
		FbxDouble3 emissiveColor(importedMaterials[importedMesh.material_Id[i]].incandescence[0], importedMaterials[importedMesh.material_Id[i]].incandescence[1], importedMaterials[importedMesh.material_Id[i]].incandescence[2]);
		FbxDouble3 transparencyColor(importedMaterials[importedMesh.material_Id[i]].transparency_color[0], importedMaterials[importedMesh.material_Id[i]].transparency_color[1], importedMaterials[importedMesh.material_Id[i]].transparency_color[2]);
		FbxDouble3 specularColor(importedMaterials[importedMesh.material_Id[i]].specular[0], importedMaterials[importedMesh.material_Id[i]].specular[1] , importedMaterials[importedMesh.material_Id[i]].specular[2]);

		const char* pathName = "C://Users/Litet/Documents/GitHub/SmallGameProject/FBX Export/FBX Export/";
		// Lambert
		if (importedMaterials[importedMesh.material_Id[i]].mtrl_type == 0)
		{
			shadingName = "Lambert";
			FbxSurfaceLambert* material = NULL;
			material= node->GetSrcObject<FbxSurfaceLambert>(0);
			material = FbxSurfaceLambert::Create(scene, materialName.Buffer());
			
			materialName += i;
			material->Emissive.Set(emissiveColor);

			material->Ambient.Set(ambientColor);

			material->Diffuse.Set(diffuseColor);
			material->DiffuseFactor.Set(importedMaterials[importedMesh.material_Id[i]].diffuse_factor);

			material->TransparentColor.Set(transparencyColor);

			FbxNode* node = mesh->GetNode();
			if (node)
			{
				node->AddMaterial(material);
			}

			// Diffuse Texture
			FbxFileTexture* texture = FbxFileTexture::Create(scene, "Diffuse Texture");
			std::cout << "DAFUSE MAP LENGTH: " << importedMaterials[importedMesh.material_Id[i]].duffuse_map_length << std::endl;
			if (importedMaterials[importedMesh.material_Id[i]].duffuse_map_length > 0)
			{
				std::string tmp(pathName);
				tmp += importedMaterials[importedMesh.material_Id[i]].diffuse_map;
				texture->SetFileName(tmp.c_str());
				texture->SetTextureUse(FbxTexture::eStandard);
				texture->SetMappingType(FbxTexture::eUV);
				texture->SetMaterialUse(FbxFileTexture::eModelMaterial);
				texture->SetSwapUV(false);
				texture->SetTranslation(0.0, 0.0);
				texture->SetScale(1.0, 1.0);
				texture->SetRotation(0.0, 0.0);

				if (material)
					material->Diffuse.ConnectSrcObject(texture);
			}

			// Normal Texture
			texture = FbxFileTexture::Create(scene, "Normal Texture");
			if (importedMaterials[importedMesh.material_Id[i]].normal_map_length > 0)
			{
				texture->SetFileName(importedMaterials[importedMesh.material_Id[i]].normal_map);
				texture->SetTextureUse(FbxTexture::eStandard);
				texture->SetMappingType(FbxTexture::eUV);
				texture->SetMaterialUse(FbxFileTexture::eModelMaterial);
				texture->SetSwapUV(false);
				texture->SetTranslation(0.0, 0.0);
				texture->SetScale(1.0, 1.0);
				texture->SetRotation(0.0, 0.0);

				if (material)
					material->NormalMap.ConnectSrcObject(texture);
			}

			

		}

		// Phong
		else
		{
			shadingName = "Phong";
			FbxSurfacePhong* material = NULL;
			material = node->GetSrcObject<FbxSurfacePhong>(0);
			material = FbxSurfacePhong::Create(scene, materialName.Buffer());
			materialName += i;
			material->Emissive.Set(emissiveColor);

			material->Ambient.Set(ambientColor);

			material->Diffuse.Set(diffuseColor);
			material->DiffuseFactor.Set(importedMaterials[importedMesh.material_Id[i]].diffuse_factor);

			material->TransparentColor.Set(transparencyColor);

			material->Specular.Set(specularColor);
			material->SpecularFactor.Set(importedMaterials[importedMesh.material_Id[i]].specular_factor);

			material->Shininess.Set(importedMaterials[importedMesh.material_Id[i]].shininess);

			FbxNode* node = mesh->GetNode();
			if (node)
			{
				node->AddMaterial(material);
			}

			// Diffuse Texture
			FbxFileTexture* texture = FbxFileTexture::Create(scene, "Diffuse Texture");
			if (importedMaterials[importedMesh.material_Id[i]].duffuse_map_length > 0)
			{
				texture->SetFileName(importedMaterials[importedMesh.material_Id[i]].diffuse_map);
				texture->SetTextureUse(FbxTexture::eStandard);
				texture->SetMappingType(FbxTexture::eUV);
				texture->SetMaterialUse(FbxFileTexture::eModelMaterial);
				texture->SetSwapUV(false);
				texture->SetTranslation(0.0, 0.0);
				texture->SetScale(1.0, 1.0);
				texture->SetRotation(0.0, 0.0);

				if (material)
					material->Diffuse.ConnectSrcObject(texture);
			}

			// Specular Texture
			texture = FbxFileTexture::Create(scene, "Specular Texture");
			if (importedMaterials[importedMesh.material_Id[i]].specular_map_length > 0)
			{
				texture->SetFileName(importedMaterials[importedMesh.material_Id[i]].specular_map);
				texture->SetTextureUse(FbxTexture::eStandard);
				texture->SetMappingType(FbxTexture::eUV);
				texture->SetMaterialUse(FbxFileTexture::eModelMaterial);
				texture->SetSwapUV(false);
				texture->SetTranslation(0.0, 0.0);
				texture->SetScale(1.0, 1.0);
				texture->SetRotation(0.0, 0.0);

				if (material)
					material->Specular.ConnectSrcObject(texture);
			}

			// Normal Texture
			texture = FbxFileTexture::Create(scene, "Normal Texture");
			if (importedMaterials[importedMesh.material_Id[i]].normal_map_length > 0)
			{
				texture->SetFileName(importedMaterials[importedMesh.material_Id[i]].normal_map);
				texture->SetTextureUse(FbxTexture::eStandard);
				texture->SetMappingType(FbxTexture::eUV);
				texture->SetMaterialUse(FbxFileTexture::eModelMaterial);
				texture->SetSwapUV(false);
				texture->SetTranslation(0.0, 0.0);
				texture->SetScale(1.0, 1.0);
				texture->SetRotation(0.0, 0.0);

				if (material)
					material->NormalMap.ConnectSrcObject(texture);
			}


		}
	}

}