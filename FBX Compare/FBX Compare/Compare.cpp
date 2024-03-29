#include "Compare.h"

Compare::Compare(Scenes* pScenes)
{
	this->lScenes = pScenes;
}

Compare::~Compare()
{

}

void Compare::DoComparison(std::string pOutputFile)
{
	FbxNode* lGoldenRootNode = this->lScenes->GetGolden()->GetRootNode();
	FbxNode* lTestRootNode = this->lScenes->GetTest()->GetRootNode();

	this->GatherInfo(lGoldenRootNode, lTestRootNode);

	this->MeshCompare();
	this->CameraCompare();
	this->LightCompare();
	this->MaterialCompare();
	this->HierarchyCompare();
}

void Compare::MeshCompare()
{
	if (this->lGoldMeshInfo.size() != this->lTestMeshInfo.size())
	{
		FBXSDK_printf("\nThe number of meshes differ between files\n");
		FBXSDK_printf("Early error out!\n");
		return;
	}
	else
	{
		for (unsigned int i = 0; i < this->lGoldMeshInfo.size(); i++)
		{
			for (unsigned int j = 0; j < this->lGoldMeshInfo[i].normals.size(); j++)
			{
				double comp_x = this->lGoldMeshInfo[i].normals[j][0];
				double comp_y = this->lGoldMeshInfo[i].normals[j][1];
				double comp_z = this->lGoldMeshInfo[i].normals[j][2];
				std::vector<FbxVector4>::iterator it = this->lTestMeshInfo[i].normals.begin();

				while (it <= this->lTestMeshInfo[i].normals.end())
				{
					it = std::find_if(it, this->lTestMeshInfo[i].normals.end(), [&, comp_x](const FbxVector4& element){return (abs(element[0] - comp_x) < EPSILON); });
					if (abs((*it)[1] - comp_y) < EPSILON && abs((*it)[2] - comp_z) < EPSILON)
					{
						break;
					}
					if (it == this->lTestMeshInfo[i].normals.end())
					{
						FBXSDK_printf("Mesh [%d], normal [%d] differ by more than an epsilon.", i, j);
						break;
					}
					it++;
				}
			}
		
			for (unsigned int j = 0; j < this->lGoldMeshInfo[i].position.size(); j++)
			{
				double comp_x = this->lGoldMeshInfo[i].position[j][0];
				double comp_y = this->lGoldMeshInfo[i].position[j][1];
				double comp_z = this->lGoldMeshInfo[i].position[j][2];
				std::vector<FbxVector4>::iterator it = this->lTestMeshInfo[i].position.begin();

				while (it <= this->lTestMeshInfo[i].position.end())
				{
					it = std::find_if(it, this->lTestMeshInfo[i].position.end(), [&, comp_x](const FbxVector4& element){return (abs(element[0] - comp_x) < EPSILON); });
					if (abs((*it)[1] - comp_y) < EPSILON && abs((*it)[2] - comp_z) < EPSILON)
					{
						break;
					}
					if (it == this->lTestMeshInfo[i].position.end())
					{
						FBXSDK_printf("Mesh [%d], vert pos [%d] differ by more than an epsilon.", i, j);
						break;
					}
					it++;
				}
			}

			for (unsigned int j = 0; j < this->lGoldMeshInfo[i].uv.size(); j++)
			{
				double comp_x = this->lGoldMeshInfo[i].uv[j][0];
				double comp_y = this->lGoldMeshInfo[i].uv[j][1];
				std::vector<FbxVector2>::iterator it = this->lTestMeshInfo[i].uv.begin();

				while (it <= this->lTestMeshInfo[i].uv.end())
				{
					it = std::find_if(it, this->lTestMeshInfo[i].uv.end(), [&, comp_x](const FbxVector2& element){return (abs(element[0] - comp_x) < EPSILON); });
					if (abs((*it)[1] - comp_y) < EPSILON)
					{
						break;
					}
					if (it == this->lTestMeshInfo[i].uv.end())
					{
						FBXSDK_printf("Mesh [%d], UV coord [%d] differ by more than an epsilon.", i, j);
						break;
					}
					it++;
				}
			}
		}

		//for (unsigned int i = 0; i < this->lGoldMeshInfo.size(); i++)
		//{
		//	for (unsigned int j = 0; j < this->lGoldMeshInfo[i].normals.size(); j++)
		//	{
		//		if (this->lGoldMeshInfo[i].normals.size() != this->lTestMeshInfo[i].normals.size())
		//		{
		//			FBXSDK_printf("\nThe number of normals differ in mesh number %d\n", i);
		//			FBXSDK_printf("Early error out!\n");
		//		}
		//		else
		//		{
		//			for (unsigned int k = 0; k < 4; k++)
		//			{
		//				if ((abs(this->lGoldMeshInfo[i].normals[j][k]) - abs(this->lTestMeshInfo[i].normals[j][k])) > EPSILON || (abs(this->lGoldMeshInfo[i].normals[j][k]) - abs(this->lTestMeshInfo[i].normals[j][k])) < -EPSILON)
		//				{
		//					FBXSDK_printf("Mesh [%d], normal [%d], coord %s differ by more than an epsilon: % f\n", i, j, this->ReturnXYZW(k), abs(abs(this->lGoldMeshInfo[i].normals[j][k]) - abs(this->lTestMeshInfo[i].normals[j][k])));
		//				}
		//			}
		//		}
		//	}

		//	for (unsigned int j = 0; j < this->lGoldMeshInfo[i].position.size(); j++)
		//	{
		//		if (this->lGoldMeshInfo[i].position.size() != this->lTestMeshInfo[i].position.size())
		//		{
		//			FBXSDK_printf("\nThe number of vertex positions differ in mesh number %d\n", i);
		//			FBXSDK_printf("Early error out!\n");
		//		}
		//		else
		//		{
		//			for (unsigned int k = 0; k < 4; k++)
		//			{
		//				if ((abs(this->lGoldMeshInfo[i].position[j][k]) - abs(this->lTestMeshInfo[i].position[j][k])) > EPSILON || (abs(this->lGoldMeshInfo[i].position[j][k]) - abs(this->lTestMeshInfo[i].position[j][k])) < -EPSILON)
		//				{
		//					FBXSDK_printf("Mesh [%d], vert pos [%d], coord %s differ by more than an epsilon: % f\n", i, j, this->ReturnXYZW(k), abs(abs(this->lGoldMeshInfo[i].position[j][k]) - abs(this->lTestMeshInfo[i].position[j][k])));
		//				}
		//			}
		//		}
		//	}

		//	for (unsigned int j = 0; j < this->lGoldMeshInfo[i].uv.size(); j++)
		//	{
		//		if (this->lGoldMeshInfo[i].uv.size() != this->lTestMeshInfo[i].uv.size())
		//		{
		//			FBXSDK_printf("\nThe number of UV coordinates differ in mesh number %d\n", i);
		//			FBXSDK_printf("Early error out!\n");
		//		}
		//		else
		//		{
		//			for (unsigned int k = 0; k < 2; k++)
		//			{
		//				if ((abs(this->lGoldMeshInfo[i].uv[j][k]) - abs(this->lTestMeshInfo[i].uv[j][k])) > EPSILON || (abs(this->lGoldMeshInfo[i].uv[j][k]) - abs(this->lTestMeshInfo[i].uv[j][k])) < -EPSILON)
		//				{
		//					FBXSDK_printf("Mesh [%d], UV [%d], coord %s differ by more than an epsilon: % f\n", i, j, this->ReturnXYZW(k), abs(abs(this->lGoldMeshInfo[i].uv[j][k]) - abs(this->lTestMeshInfo[i].uv[j][k])));
		//				}
		//			}
		//		}
		//	}
		//}
	}
}

void Compare::CameraCompare()
{
	if (this->lGoldCamInfo.size() != this->lTestCamInfo.size())
	{
		FBXSDK_printf("\nThe number of cameras differ between files\n");
		FBXSDK_printf("Early error out!\n");
		return;
	}
	else
	{
		for (unsigned int i = 0; i < this->lGoldCamInfo.size(); i++)
		{
			//for (unsigned int j = 0; j < 3; j++)
			//{
			//	if ((abs(this->lGoldCamInfo[i].position[j]) - abs(this->lTestCamInfo[i].position[j])) > EPSILON || (abs(this->lGoldCamInfo[i].position[j]) - abs(this->lTestCamInfo[i].position[j])) < -EPSILON)
			//	{
			//		FBXSDK_printf("Camera [%d], %s-position differ by more than an epsilon: % f\n", i, this->ReturnXYZW(j), abs(abs(this->lGoldCamInfo[i].position[j]) - abs(this->lTestCamInfo[i].position[j])));
			//	}
			//}
			//for (unsigned int j = 0; j < 3; j++)
			//{
			//	if ((abs(this->lGoldCamInfo[i].up_vector[j]) - abs(this->lTestCamInfo[i].up_vector[j])) > EPSILON || (abs(this->lGoldCamInfo[i].up_vector[j]) - abs(this->lTestCamInfo[i].up_vector[j])) < -EPSILON)
			//	{
			//		FBXSDK_printf("Camera [%d], up-vector %s-value differ by more than an epsilon: % f\n", i, this->ReturnXYZW(j), abs(abs(this->lGoldCamInfo[i].up_vector[j]) - abs(this->lTestCamInfo[i].up_vector[j])));
			//	}
			//}
			//for (unsigned int j = 0; j < 3; j++)
			//{
			//	if ((abs(this->lGoldCamInfo[i].interest_position[j]) - abs(this->lTestCamInfo[i].interest_position[j])) > EPSILON || (abs(this->lGoldCamInfo[i].interest_position[j]) - abs(this->lTestCamInfo[i].interest_position[j])) < -EPSILON)
			//	{
			//		FBXSDK_printf("Camera [%d], interest position %s-value differ by more than an epsilon: % f\n", i, this->ReturnXYZW(j), abs(abs(this->lGoldCamInfo[i].interest_position[j]) - abs(this->lTestCamInfo[i].interest_position[j])));
			//	}
			//}
			//if ((abs(this->lGoldCamInfo[i].field_of_view_x) - abs(this->lTestCamInfo[i].field_of_view_x)) > EPSILON || (abs(this->lGoldCamInfo[i].field_of_view_x) - abs(this->lTestCamInfo[i].field_of_view_x)) < -EPSILON)
			//{
			//	FBXSDK_printf("Camera [%d], FOV x-value differ by more than an epsilon: % f\n", i, abs(abs(this->lGoldCamInfo[i].field_of_view_x) - abs(this->lTestCamInfo[i].field_of_view_x)));
			//}
			//if ((abs(this->lGoldCamInfo[i].field_of_view_y) - abs(this->lTestCamInfo[i].field_of_view_y)) > EPSILON || (abs(this->lGoldCamInfo[i].field_of_view_y) - abs(this->lTestCamInfo[i].field_of_view_y)) < -EPSILON)
			//{
			//	FBXSDK_printf("Camera [%d], FOV y-value differ by more than an epsilon: % f\n", i, abs(abs(this->lGoldCamInfo[i].field_of_view_y) - abs(this->lTestCamInfo[i].field_of_view_y)));
			//}
			if ((abs(this->lGoldCamInfo[i].near_plane) - abs(this->lTestCamInfo[i].near_plane)) > EPSILON || (abs(this->lGoldCamInfo[i].near_plane) - abs(this->lTestCamInfo[i].near_plane)) < -EPSILON)
			{
				FBXSDK_printf("Camera [%d], near clip plane differ by more than an epsilon: % f\n", i, abs(abs(this->lGoldCamInfo[i].near_plane) - abs(this->lTestCamInfo[i].near_plane)));
			}
			if ((abs(this->lGoldCamInfo[i].far_plane) - abs(this->lTestCamInfo[i].far_plane)) > EPSILON || (abs(this->lGoldCamInfo[i].far_plane) - abs(this->lTestCamInfo[i].far_plane)) < -EPSILON)
			{
				FBXSDK_printf("Camera [%d], far clip plane differ by more than an epsilon: % f\n", i, abs(abs(this->lGoldCamInfo[i].far_plane) - abs(this->lTestCamInfo[i].far_plane)));
			}
			if (this->lGoldCamInfo[i].projection != this->lTestCamInfo[i].projection)
			{
				FBXSDK_printf("Camera [%d], projection type differ\n", i);
			}
		}
	}
}

void Compare::LightCompare()
{
	if (this->lGoldLightInfo.size() != this->lTestLightInfo.size())
	{
		FBXSDK_printf("\nThe number of lights differ between files\n");
		FBXSDK_printf("Early error out!\n");
		return;
	}
	else
	{
		for (unsigned int i = 0; i < this->lGoldLightInfo.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				if ((abs(this->lGoldLightInfo[i].color[j]) - abs(this->lTestLightInfo[i].color[j])) > EPSILON || (abs(this->lGoldLightInfo[i].color[j]) - abs(this->lTestLightInfo[i].color[j])) < -EPSILON)
				{
					FBXSDK_printf("Light [%d], color %s differ by more than an epsilon: % f\n", i, this->ReturnRGBA(j), abs(abs(this->lGoldLightInfo[i].color[j]) - abs(this->lTestLightInfo[i].color[j])));
				}
			}
			for (unsigned int j = 0; j < 3; j++)
			{
				if ((abs(this->lGoldLightInfo[i].shadow_color[j]) - abs(this->lTestLightInfo[i].shadow_color[j])) > EPSILON || (abs(this->lGoldLightInfo[i].shadow_color[j]) - abs(this->lTestLightInfo[i].shadow_color[j])) < -EPSILON)
				{
					FBXSDK_printf("Light [%d], shadow color %s differ by more than an epsilon: % f\n", i, this->ReturnRGBA(j), abs(abs(this->lGoldLightInfo[i].shadow_color[j]) - abs(this->lTestLightInfo[i].shadow_color[j])));
				}
			}
			if ((abs(this->lGoldLightInfo[i].intensity) - abs(this->lTestLightInfo[i].intensity)) > EPSILON || (abs(this->lGoldLightInfo[i].intensity) - abs(this->lTestLightInfo[i].intensity)) < -EPSILON)
			{
				FBXSDK_printf("Light [%d], intensity differ by more than an epsilon: % f\n", i, abs(abs(this->lGoldLightInfo[i].intensity) - abs(this->lTestLightInfo[i].intensity)));
			}
			if (this->lGoldLightInfo[i].decay_type != this->lTestLightInfo[i].decay_type)
			{
				FBXSDK_printf("Light [%d], decay type differ\n", i);
			}
			if (this->lGoldLightInfo[i].light_type != this->lTestLightInfo[i].light_type)
			{
				FBXSDK_printf("Light [%d], light type differ\n", i);
			}
			if (this->lGoldLightInfo[i].cast_shadows != this->lTestLightInfo[i].cast_shadows)
			{
				FBXSDK_printf("Light [%d], only one light casts shadows\n", i);
			}
		}
	}
}

void Compare::MaterialCompare()
{
	if (this->lGoldMaterialInfo.size() != this->lTestMaterialInfo.size())
	{
		FBXSDK_printf("\nThe number of materials differ between files\n");
		FBXSDK_printf("Early error out!\n");
		return;
	}
	else
	{
		for (unsigned int i = 0; i < this->lGoldMaterialInfo.size(); i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				if ((abs(this->lGoldMaterialInfo[i].second.specular[j]) - abs(this->lTestMaterialInfo[i].second.specular[j])) > EPSILON || (abs(this->lGoldMaterialInfo[i].second.specular[j]) - abs(this->lTestMaterialInfo[i].second.specular[j])) < -EPSILON)
				{
					FBXSDK_printf("Material %s, specular color %s differ by more than an epsilon: % f\n", lGoldMaterialInfo[i].first, this->ReturnRGBA(j), abs(abs(this->lGoldMaterialInfo[i].second.specular[j]) - abs(this->lTestMaterialInfo[i].second.specular[j])));
				}
			}
			for (unsigned int j = 0; j < 3; j++)
			{
				if ((abs(this->lGoldMaterialInfo[i].second.reflection[j]) - abs(this->lTestMaterialInfo[i].second.reflection[j])) > EPSILON || (abs(this->lGoldMaterialInfo[i].second.reflection[j]) - abs(this->lTestMaterialInfo[i].second.reflection[j])) < -EPSILON)
				{
					FBXSDK_printf("Material %s, reflection color %s differ by more than an epsilon: % f\n", lGoldMaterialInfo[i].first, this->ReturnRGBA(j), abs(abs(this->lGoldMaterialInfo[i].second.reflection[j]) - abs(this->lTestMaterialInfo[i].second.reflection[j])));
				}
			}
			for (unsigned int j = 0; j < 3; j++)
			{
				if ((abs(this->lGoldMaterialInfo[i].second.ambient[j]) - abs(this->lTestMaterialInfo[i].second.ambient[j])) > EPSILON || (abs(this->lGoldMaterialInfo[i].second.ambient[j]) - abs(this->lTestMaterialInfo[i].second.ambient[j])) < -EPSILON)
				{
					FBXSDK_printf("Material %s, ambient color %s differ by more than an epsilon: % f\n", lGoldMaterialInfo[i].first, this->ReturnRGBA(j), abs(abs(this->lGoldMaterialInfo[i].second.ambient[j]) - abs(this->lTestMaterialInfo[i].second.ambient[j])));
				}
			}
			for (unsigned int j = 0; j < 3; j++)
			{
				if ((abs(this->lGoldMaterialInfo[i].second.diffuse[j]) - abs(this->lTestMaterialInfo[i].second.diffuse[j])) > EPSILON || (abs(this->lGoldMaterialInfo[i].second.diffuse[j]) - abs(this->lTestMaterialInfo[i].second.diffuse[j])) < -EPSILON)
				{
					FBXSDK_printf("Material %s, diffuse color %s differ by more than an epsilon: % f\n", lGoldMaterialInfo[i].first, this->ReturnRGBA(j), abs(abs(this->lGoldMaterialInfo[i].second.diffuse[j]) - abs(this->lTestMaterialInfo[i].second.diffuse[j])));
				}
			}
			for (unsigned int j = 0; j < 3; j++)
			{
				if ((abs(this->lGoldMaterialInfo[i].second.transparency_color[j]) - abs(this->lTestMaterialInfo[i].second.transparency_color[j])) > EPSILON || (abs(this->lGoldMaterialInfo[i].second.transparency_color[j]) - abs(this->lTestMaterialInfo[i].second.transparency_color[j])) < -EPSILON)
				{
					FBXSDK_printf("Material %s, transparency color %s differ by more than an epsilon: % f\n", lGoldMaterialInfo[i].first, this->ReturnRGBA(j), abs(abs(this->lGoldMaterialInfo[i].second.transparency_color[j]) - abs(this->lTestMaterialInfo[i].second.transparency_color[j])));
				}
			}
			for (unsigned int j = 0; j < 3; j++)
			{
				if ((abs(this->lGoldMaterialInfo[i].second.incandescence[j]) - abs(this->lTestMaterialInfo[i].second.incandescence[j])) > EPSILON || (abs(this->lGoldMaterialInfo[i].second.incandescence[j]) - abs(this->lTestMaterialInfo[i].second.incandescence[j])) < -EPSILON)
				{
					FBXSDK_printf("Material %s, incandescence color %s differ by more than an epsilon: % f\n", lGoldMaterialInfo[i].first, this->ReturnRGBA(j), abs(abs(this->lGoldMaterialInfo[i].second.incandescence[j]) - abs(this->lTestMaterialInfo[i].second.incandescence[j])));
				}
			}
			if ((abs(this->lGoldMaterialInfo[i].second.specular_factor) - abs(this->lTestMaterialInfo[i].second.specular_factor)) > EPSILON || (abs(this->lGoldMaterialInfo[i].second.specular_factor) - abs(this->lTestMaterialInfo[i].second.specular_factor)) < -EPSILON)
			{
				FBXSDK_printf("Material %s, specular factor differ by more than an epsilon: % f\n", lGoldMaterialInfo[i].first, abs(abs(this->lGoldMaterialInfo[i].second.specular_factor) - abs(this->lTestMaterialInfo[i].second.specular_factor)));
			}
			if ((abs(this->lGoldMaterialInfo[i].second.shininess) - abs(this->lTestMaterialInfo[i].second.shininess)) > EPSILON || (abs(this->lGoldMaterialInfo[i].second.shininess) - abs(this->lTestMaterialInfo[i].second.shininess)) < -EPSILON)
			{
				FBXSDK_printf("Material %s, shininess differ by more than an epsilon: % f\n", lGoldMaterialInfo[i].first, abs(abs(this->lGoldMaterialInfo[i].second.shininess) - abs(this->lTestMaterialInfo[i].second.shininess)));
			}
			if ((abs(this->lGoldMaterialInfo[i].second.reflection_factor) - abs(this->lTestMaterialInfo[i].second.reflection_factor)) > EPSILON || (abs(this->lGoldMaterialInfo[i].second.reflection_factor) - abs(this->lTestMaterialInfo[i].second.reflection_factor)) < -EPSILON)
			{
				FBXSDK_printf("Material %s, reflection factor differ by more than an epsilon: % f\n", lGoldMaterialInfo[i].first, abs(abs(this->lGoldMaterialInfo[i].second.reflection_factor) - abs(this->lTestMaterialInfo[i].second.reflection_factor)));
			}
			if ((abs(this->lGoldMaterialInfo[i].second.diffuse_factor) - abs(this->lTestMaterialInfo[i].second.diffuse_factor)) > EPSILON || (abs(this->lGoldMaterialInfo[i].second.diffuse_factor) - abs(this->lTestMaterialInfo[i].second.diffuse_factor)) < -EPSILON)
			{
				FBXSDK_printf("Material %s, diffuse factor differ by more than an epsilon: % f\n", lGoldMaterialInfo[i].first, abs(abs(this->lGoldMaterialInfo[i].second.diffuse_factor) - abs(this->lTestMaterialInfo[i].second.diffuse_factor)));
			}
			if (this->lGoldMaterialInfo[i].second.material_type != this->lTestMaterialInfo[i].second.material_type)
			{
				FBXSDK_printf("Material %s, material type differ\n", lGoldMaterialInfo[i].first);
			}
		}
	}
}

void Compare::HierarchyCompare()
{
	if (this->lGoldHierarchyNodes.size() != this->lTestHierarchyNodes.size())
	{
		FBXSDK_printf("\nThe number of hierarchial nodes differ between files\n");
		FBXSDK_printf("Early error out!\n");
		return;
	}
	else
	{
		bool nameDiffer = false;
		bool childDiffer = false;
		std::vector<std::pair<unsigned int, unsigned int>> indexStore;

		for (unsigned int i = 0; i < this->lGoldHierarchyNodes.size(); i++)
		{
			std::string temp_name = this->lGoldHierarchyNodes[i].node_name;

			std::vector<TransformNodeInfo>::iterator it = std::find_if(this->lTestHierarchyNodes.begin(), this->lTestHierarchyNodes.end(), [&, temp_name](const TransformNodeInfo& element){ return element.node_name == temp_name; });

			indexStore.push_back(std::make_pair(i, it - this->lTestHierarchyNodes.begin()));

			if (it - this->lTestHierarchyNodes.begin() + 1 > this->lGoldHierarchyNodes.size())
			{
				nameDiffer = true;
				FBXSDK_printf("The node %s was not found in the test file\n", temp_name);
			}
		}
		if (nameDiffer == false)
		{
			for (unsigned int i = 0; i < indexStore.size(); i++)
			{
				if (this->lGoldHierarchyNodes[indexStore[i].first].child_names.size() != this->lTestHierarchyNodes[indexStore[i].second].child_names.size())
				{
					FBXSDK_printf("The nodes %s does not have the same amount of children: %d vs %d\n", 
						this->lGoldHierarchyNodes[indexStore[i].first].node_name, 
						this->lGoldHierarchyNodes[indexStore[i].first].child_names.size(), 
						this->lTestHierarchyNodes[indexStore[i].second].child_names.size());
					
					childDiffer = true;
				}
			}
			if (childDiffer == false)
			{
				for (unsigned int i = 0; i < this->lGoldHierarchyNodes.size(); i++)
				{
					for (unsigned int j = 0; j < 3; j++)
					{
						if ((abs(this->lGoldHierarchyNodes[i].rotation[j]) - abs(this->lTestHierarchyNodes[i].rotation[j])) > EPSILON || (abs(this->lGoldHierarchyNodes[i].rotation[j]) - abs(this->lTestHierarchyNodes[i].rotation[j])) < -EPSILON)
						{
							FBXSDK_printf("Node %s, rotation %s differ by more than an epsilon: % f\n", this->lGoldHierarchyNodes[i].node_name, this->ReturnXYZW(j), abs(abs(this->lGoldHierarchyNodes[i].rotation[j]) - abs(this->lTestHierarchyNodes[i].rotation[j])));
						}
					}
					for (unsigned int j = 0; j < 3; j++)
					{
						if ((abs(this->lGoldHierarchyNodes[i].scale[j]) - abs(this->lTestHierarchyNodes[i].scale[j])) > EPSILON || (abs(this->lGoldHierarchyNodes[i].scale[j]) - abs(this->lTestHierarchyNodes[i].scale[j])) < -EPSILON)
						{
							FBXSDK_printf("Node %s, scale %s differ by more than an epsilon: % f\n", this->lGoldHierarchyNodes[i].node_name, this->ReturnXYZW(j), abs(abs(this->lGoldHierarchyNodes[i].scale[j]) - abs(this->lTestHierarchyNodes[i].scale[j])));
						}
					}
					for (unsigned int j = 0; j < 3; j++)
					{
						if ((abs(this->lGoldHierarchyNodes[i].translation[j]) - abs(this->lTestHierarchyNodes[i].translation[j])) > EPSILON || (abs(this->lGoldHierarchyNodes[i].translation[j]) - abs(this->lTestHierarchyNodes[i].translation[j])) < -EPSILON)
						{
							FBXSDK_printf("Node %s, translation %s differ by more than an epsilon: % f\n", this->lGoldHierarchyNodes[i].node_name, this->ReturnXYZW(j), abs(abs(this->lGoldHierarchyNodes[i].translation[j]) - abs(this->lTestHierarchyNodes[i].translation[j])));
						}
					}
				}
			}
		}
	}
}

void Compare::GatherInfo(FbxNode* pGoldenRootNode, FbxNode* pTestRootNode)
{
	MeshInfo tempMeshInfo;
	CamInfo tempCamInfo;
	LightInfo tempLightInfo;
	TransformNodeInfo tempTransformNodeInfo;

	int counter;

	// GOLD
	for (counter = 0; counter <= pGoldenRootNode->GetChildCount(); counter++)
	{
		this->TraverseScene(pGoldenRootNode->GetChild(counter), this->isGolden);
	}

	// TEST
	for (counter = 0; counter <= pTestRootNode->GetChildCount(); counter++)
	{
		this->TraverseScene(pTestRootNode->GetChild(counter), this->isTest);
	}
}

void Compare::TraverseScene(FbxNode* pNode, bool pType)
{
	int counter;
	MeshInfo tempMeshInfo;
	CamInfo tempCamInfo;
	LightInfo tempLightInfo;
	TransformNodeInfo tempTransformNodeInfo;

	if (pNode)
	{		
		// MESH
		this->lStatus = this->lMeshHandler.GetInfo(pNode, tempMeshInfo);
		if (lStatus)
		{
			if (pType == this->isGolden)
			{
				this->lGoldMeshInfo.push_back(tempMeshInfo);
				tempMeshInfo.normals.clear();
				tempMeshInfo.position.clear();
				tempMeshInfo.uv.clear();
			}
			else
			{
				this->lTestMeshInfo.push_back(tempMeshInfo);
				tempMeshInfo.normals.clear();
				tempMeshInfo.position.clear();
				tempMeshInfo.uv.clear();
			}
		}
		// CAMERA
		this->lStatus = this->lCamHandler.GetInfo(pNode, tempCamInfo);
		if (lStatus)
		{
			if (pType == this->isGolden)
			{
				this->lGoldCamInfo.push_back(tempCamInfo);
			}
			else
			{
				this->lTestCamInfo.push_back(tempCamInfo);
			}
		}
		// LIGHT
		this->lStatus = this->lLightHandler.GetInfo(pNode, tempLightInfo);
		if (lStatus)
		{
			if (pType == this->isGolden)
			{
				this->lGoldLightInfo.push_back(tempLightInfo);
			}
			else
			{
				this->lTestLightInfo.push_back(tempLightInfo);
			}
		}
		// MATERIAL
		if (pType == this->isGolden)
		{
			this->lStatus = this->lMaterialHandler.GetInfo(pNode, lGoldMaterialNames, lGoldMaterialInfo);
		}
		else
		{
			this->lStatus = this->lMaterialHandler.GetInfo(pNode, lTestMaterialNames, lTestMaterialInfo);
		}
		// HIERARCHY
		this->lStatus = this->lHierarchyHandler.GetInfo(pNode, tempTransformNodeInfo);
		if (lStatus)
		{
			if (pType == this->isGolden)
			{
				this->lGoldHierarchyNodes.push_back(tempTransformNodeInfo);
				tempTransformNodeInfo.child_names.clear();
			}
			else
			{
				this->lTestHierarchyNodes.push_back(tempTransformNodeInfo);
				tempTransformNodeInfo.child_names.clear();
			}
		}

		for (counter = 0; counter < pNode->GetChildCount(); counter++)
		{
			TraverseScene(pNode->GetChild(counter), pType);
		}
	}
}

std::string Compare::ReturnXYZW(int value)
{
	if (value == 0){ return "x"; }
	else if(value == 1){ return "y"; }
	else if(value == 2){ return "z"; }
	else if(value == 3){ return "w"; }
	else { return "unk"; }
}

std::string Compare::ReturnRGBA(int value)
{
	if (value == 0){ return "r"; }
	else if (value == 1){ return "g"; }
	else if (value == 2){ return "b"; }
	else if (value == 3){ return "a"; }
	else { return "unk"; }
}