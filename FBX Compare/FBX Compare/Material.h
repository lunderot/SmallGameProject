#ifndef MATERIAL_H
#define MATERIAL_H

#include "DataTypes.h"
#include <map>

class Material
{
private:
	void GetStats(FbxSurfaceMaterial* pSurfaceMaterial, std::vector<std::pair<std::string, MaterialInfo>>& pMaterialInfo);
protected:
public:
	Material();
	~Material();

	bool GetInfo(FbxNode* pNode, std::set<std::string>& pMaterialNames, std::vector<std::pair<std::string, MaterialInfo>>& pMaterialInfo);
};

#endif /* MATERIAL_H */