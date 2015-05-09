#ifndef MESH_H
#define MESH_H

#include "DataTypes.h"

class Mesh
{
private:
	void GetNormals(FbxMesh* pMesh, MeshInfo& pMeshInfo);
	void GetPosition(FbxMesh* pMesh, MeshInfo& pMeshInfo);
protected:
public:
	Mesh();
	~Mesh();

	bool GetInfo(FbxNode* pNode, MeshInfo& pMeshInfo);
};

#endif /* MESH_H */