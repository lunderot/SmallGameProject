#ifndef MESH_H
#define MESH_H

#include "maya_includes.h"
#define MAYA_EXPORT
#include "CommonDeclaration.h"
#include <fstream>
#include <map>

using namespace std;

class Mesh
{
public:
	MStatus exportMesh(MFnMesh& mesh, map<const char*, int>& materials, map<const char*, int>& transformHeiraki, meshStruct& meshes, map<const char*, unsigned int>& meshMap,
		vector<vector<double>>& position,
		vector<vector<float>>& uv,
		vector<vector<double>>& normal,
		vector<vector<double>>& tangent,
		vector<vector<double>>& bi_tangent,
		vector<vector<int>>& transform_Id,
		vector<vector<int>>& material_Id,
		vector<vector<Vertex>>& vertices,
		vector<const char*>& name);

private:
	MStatus exportMaterial(MFnMesh& mesh, map<const char*, int>& materials, meshStruct& meshes, 
		vector<vector<int>>& material_Id);

	MStatus exportVertices(MFnMesh& mesh, map<const char*, int>& transformHeiraki, map<const char*, int>& materials, meshStruct& meshes,
		vector<vector<double>>& position,
		vector<vector<float>>& uv,
		vector<vector<double>>& normal,
		vector<vector<double>>& tangent,
		vector<vector<double>>& bi_tangent,
		vector<vector<int>>& transform_Id,
		vector<vector<Vertex>>& vertices,
		vector<const char*>& name);
	MStatus exportJoints(MFnMesh& mesh);
};

#endif