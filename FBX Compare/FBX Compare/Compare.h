#ifndef COMPARE_H
#define COMPARE_H

#include "Defines.h"
#include "DataTypes.h"
#include "Scenes.h"
#include "Mesh.h"

class Compare
{
private:
	Scenes* lScenes;

	Mesh meshHandler;

	std::vector<MeshInfo> lGoldMeshInfo;
	std::vector<MeshInfo> lTestMeshInfo;
protected:
public:
	Compare(Scenes* pScenes);
	~Compare();

	void DoComparison(std::string pOutputFile);
};

#endif /* COMPARE_H */