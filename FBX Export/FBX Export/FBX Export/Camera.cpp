#include "Camera.h"

void Camera::ExportCameras(FbxScene* scene, std::string fileName)
{
	Importer importer;
	if (!importer.importFile(fileName))
	{
		FBXSDK_printf("Failed to Import the file");
	}
	const ImporterCamera* importedCameras = importer.getCamera();

	FbxNode* rootNode = scene->GetRootNode();
	
	for (unsigned int i = 0; i < importer.getNumCameras(); i++)
	{
		cout << importedCameras[i].name << endl;
		cout << std::strcmp(importedCameras[i].name, "perspShape") << endl;
		if (std::strcmp(importedCameras[i].name, "perspShape") != 0 && std::strcmp(importedCameras[i].name, "topShape") != 0 && std::strcmp(importedCameras[i].name, "frontShape") != 0 && std::strcmp(importedCameras[i].name, "sideShape") != 0)
		{
			FbxCamera* camera = FbxCamera::Create(scene, importedCameras[i].name);
			FbxNode* camNode = FbxNode::Create(scene, "");
			//camera->Position.Set(FbxDouble3(importedCameras[i].position[0], importedCameras[i].position[1], importedCameras[i].position[2]));

			camera->UpVector.Set(FbxDouble3(importedCameras[i].up_vector[0], importedCameras[i].up_vector[1], importedCameras[i].up_vector[2]));
			camera->InterestPosition.Set(FbxDouble3(importedCameras[i].interest_position[0], importedCameras[i].interest_position[1], importedCameras[i].interest_position[2]));
			cout << importedCameras[i].interest_position[0] << importedCameras[i].interest_position[1] << importedCameras[i].interest_position[2] << endl;

			camera->FieldOfViewX.Set(importedCameras[i].field_of_view_x);
			camera->FieldOfViewY.Set(importedCameras[i].field_of_view_y);

			camera->NearPlane.Set(importedCameras[i].near_plane);
			camera->FarPlane.Set(importedCameras[i].far_plane);

			if (importedCameras[i].projection == 0)
				camera->ProjectionType.Set(FbxCamera::EProjectionType::ePerspective);
			else
				camera->ProjectionType.Set(FbxCamera::EProjectionType::eOrthogonal);

			for (unsigned int j = 0; j < importedCameras[i].nrOfParents; j++)
			{
				int parentID = importedCameras[i].parentID[j] + 1;
				FbxNode* parentNode = scene->GetNode(parentID);
				parentNode->SetNodeAttribute(camera);
			}
		}
	}
}