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

		this->GetPosition(lCamera, pCamInfo);

		return true;
	}
	return false;
}

void Camera::GetPosition(FbxCamera* pCamera, CamInfo& pCamInfo)
{
	pCamInfo.position = pCamera->Position;
}