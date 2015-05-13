#include "Material.h"

Material::Material()
{

}

Material::~Material()
{

}

bool Material::GetInfo(FbxNode* pNode, std::set<std::string>& pMaterialNames, std::vector<std::pair<std::string, MaterialInfo>>& pMaterialInfo)
{
	int lMaterialCount = pNode->GetMaterialCount();

	for (int i = 0; i < lMaterialCount; i++)
	{
		FbxSurfaceMaterial* lSurfaceMaterial = pNode->GetMaterial(i);

		if (lSurfaceMaterial)
		{
			std::string temp_name = lSurfaceMaterial->GetName();

			if (pMaterialNames.find(temp_name) == pMaterialNames.end())
			{
				//FBXSDK_printf("current material node: %s\n", lSurfaceMaterial->GetName());

				pMaterialNames.insert(temp_name);

				this->GetStats(lSurfaceMaterial, pMaterialInfo);
			}
		}
	}
	return true;
}

void Material::GetStats(FbxSurfaceMaterial* pSurfaceMaterial, std::vector<std::pair<std::string, MaterialInfo>>& pMaterialInfo)
{
	std::pair<std::string, MaterialInfo> temp_pair;
	std::string temp_string;
	MaterialInfo temp_mat;

	temp_string = pSurfaceMaterial->GetName();

	if (pSurfaceMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		temp_mat.ambient = ((FbxSurfacePhong*)pSurfaceMaterial)->Ambient.Get();
		temp_mat.diffuse = ((FbxSurfacePhong*)pSurfaceMaterial)->Diffuse.Get();
		temp_mat.diffuse_factor = ((FbxSurfacePhong*)pSurfaceMaterial)->DiffuseFactor.Get();
		temp_mat.incandescence = ((FbxSurfacePhong*)pSurfaceMaterial)->Emissive.Get();
		temp_mat.material_type = 1;
		temp_mat.reflection = ((FbxSurfacePhong*)pSurfaceMaterial)->Reflection.Get();
		temp_mat.reflection_factor = ((FbxSurfacePhong*)pSurfaceMaterial)->ReflectionFactor.Get();
		temp_mat.shininess = ((FbxSurfacePhong*)pSurfaceMaterial)->Shininess.Get();
		temp_mat.specular = ((FbxSurfacePhong*)pSurfaceMaterial)->Specular.Get();
		temp_mat.specular_factor = ((FbxSurfacePhong*)pSurfaceMaterial)->SpecularFactor.Get();
		temp_mat.transparency_color = ((FbxSurfacePhong*)pSurfaceMaterial)->TransparentColor.Get();
	}
	else if (pSurfaceMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		temp_mat.ambient = ((FbxSurfaceLambert*)pSurfaceMaterial)->Ambient.Get();
		temp_mat.diffuse = ((FbxSurfaceLambert*)pSurfaceMaterial)->Diffuse.Get();
		temp_mat.diffuse_factor = ((FbxSurfaceLambert*)pSurfaceMaterial)->DiffuseFactor.Get();
		temp_mat.incandescence = ((FbxSurfaceLambert*)pSurfaceMaterial)->Emissive.Get();
		temp_mat.material_type = 0;
		temp_mat.reflection = FbxDouble3(0.0f, 0.0f, 0.0f);
		temp_mat.reflection_factor = 0.0f;
		temp_mat.shininess = 0.0f;
		temp_mat.specular = FbxDouble3(0.0f, 0.0f, 0.0f);
		temp_mat.specular_factor = 0.0f;
		temp_mat.transparency_color = ((FbxSurfaceLambert*)pSurfaceMaterial)->TransparentColor.Get();
	}

	temp_pair.first = temp_string;
	temp_pair.second = temp_mat;

	pMaterialInfo.push_back(temp_pair);
}