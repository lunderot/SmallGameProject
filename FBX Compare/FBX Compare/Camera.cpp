#include "Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::GetInfo(FbxNode* pNode, CamInfo& pCamInfo)
{
	FbxCamera* lCamera = pNode->GetCamera();

	if (lCamera)
	{
		FBXSDK_printf("current camera node: %s\n", pNode->GetName());

		this->GetStats(lCamera, pCamInfo);

		return true;
	}
	return false;
}

void Camera::GetStats(FbxCamera* pCamera, CamInfo& pCamInfo)
{
	pCamInfo.position = pCamera->Position.Get();
	pCamInfo.up_vector = pCamera->UpVector.Get();
	pCamInfo.interest_position = pCamera->InterestPosition.Get();
	pCamInfo.field_of_view_x = pCamera->FieldOfViewX.Get();
	pCamInfo.field_of_view_y = pCamera->FieldOfViewY.Get();
	pCamInfo.near_plane = pCamera->NearPlane.Get();
	pCamInfo.far_plane = pCamera->FarPlane.Get();
	pCamInfo.projection = pCamera->ProjectionType.Get();
}