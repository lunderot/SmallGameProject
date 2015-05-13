#ifndef HIERARCHY_H
#define HIERARCHY_H

#include "DataTypes.h"

class Hierarchy
{
private:
	void GetTransforms(FbxNode* pNode, TransformNodeInfo& pTransformNodeInfo);
	void GetChildren(FbxNode* pNode, TransformNodeInfo& pTransformNodeInfo);
protected:
public:
	Hierarchy();
	~Hierarchy();

	bool GetInfo(FbxNode* pNode, TransformNodeInfo& pTransformNodeInfo);
};

#endif /* HIERARCHY_H */