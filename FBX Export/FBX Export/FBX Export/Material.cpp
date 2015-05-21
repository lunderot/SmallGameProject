#include "Material.h"

void Material::ExportMaterials(FbxScene* scene, FbxMesh* mesh, const ImporterMesh& importedMesh, const ImporterMaterial* importedMaterials)
{
	for (int i = 0; i < importedMesh.material_count; i++)
	{
		FbxString materialName = importedMaterials[importedMesh.material_Id[i]].name;
		
		FbxString shadingName;

		FbxDouble3 diffuseColor(importedMaterials[importedMesh.material_Id[i]].diffuse[0], importedMaterials[importedMesh.material_Id[i]].diffuse[1], importedMaterials[importedMesh.material_Id[i]].diffuse[2]);
		FbxDouble3 ambientColor(importedMaterials[importedMesh.material_Id[i]].ambient[0], importedMaterials[importedMesh.material_Id[i]].ambient[1], importedMaterials[importedMesh.material_Id[i]].ambient[2]);
		FbxDouble3 emissiveColor(importedMaterials[importedMesh.material_Id[i]].incandescence[0], importedMaterials[importedMesh.material_Id[i]].incandescence[1], importedMaterials[importedMesh.material_Id[i]].incandescence[2]);
		FbxDouble3 transparencyColor(importedMaterials[importedMesh.material_Id[i]].transparency_color[0], importedMaterials[importedMesh.material_Id[i]].transparency_color[1], importedMaterials[importedMesh.material_Id[i]].transparency_color[2]);
		FbxDouble3 specularColor(importedMaterials[importedMesh.material_Id[i]].specular[0], importedMaterials[importedMesh.material_Id[i]].specular[1] , importedMaterials[importedMesh.material_Id[i]].specular[2]);

		if (importedMaterials[importedMesh.material_Id[i]].mtrl_type == 0)
		{
			shadingName = "Lambert";
			FbxSurfaceLambert* material = FbxSurfaceLambert::Create(scene, materialName.Buffer());
			material->Emissive.Set(emissiveColor);

			material->Ambient.Set(ambientColor);

			material->Diffuse.Set(diffuseColor);
			material->DiffuseFactor.Set(importedMaterials[importedMesh.material_Id[i]].diffuse_factor);

			material->TransparentColor.Set(transparencyColor);

			FbxNode* node = mesh->GetNode();
			if (node)
				node->AddMaterial(material);
		}
		else if (importedMaterials[importedMesh.material_Id[i]].mtrl_type == 1)
		{
			shadingName = "Blinn";

			FbxSurfacePhong* material = FbxSurfacePhong::Create(scene, materialName.Buffer());
			

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
				node->AddMaterial(material);

		}
		else
		{
			shadingName = "Phong";
			FbxSurfacePhong* material = FbxSurfacePhong::Create(scene, materialName.Buffer());

			material->Emissive.Set(emissiveColor);

			material->Ambient.Set(ambientColor);

			material->Diffuse.Set(diffuseColor);
			material->DiffuseFactor.Set(importedMaterials[importedMesh.material_Id[i]].diffuse_factor);

			material->TransparentColor.Set(transparencyColor);

			material->Specular.Set(specularColor);
			material->SpecularFactor.Set(importedMaterials[importedMesh.material_Id[i]].specular_factor);

			material->Shininess.Set(importedMaterials[importedMesh.material_Id[i]].shininess);
			std::cout << "MATERIAL INC!!!" << std::endl;

			FbxNode* node = mesh->GetNode();
			if (node)
				node->AddMaterial(material);

		}

		materialName += i;
		std::cout << "MATERIAL INC!!!" << std::endl;
	}

}