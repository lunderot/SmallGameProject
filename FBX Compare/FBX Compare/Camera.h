#ifndef CAMERA_H
#define CAMERA_H

#include "DataTypes.h"

class Camera
{
private:
	void GetStats(FbxCamera* pCamera, CamInfo& pCamInfo);
protected:
public:
	Camera();
	~Camera();

	bool GetInfo(FbxNode* pNode, CamInfo& pCamInfo);
};

#endif /* CAMERA_H */