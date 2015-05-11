#ifndef LIGHT_H
#define LIGHT_H

#include "DataTypes.h"

class Light
{
private:
	void GetStats(FbxLight* pLight, LightInfo& pLightInfo);
protected:
public:
	Light();
	~Light();

	bool GetInfo(FbxNode* pNode, LightInfo& pLightInfo);
};

#endif /* LIGHT_H */