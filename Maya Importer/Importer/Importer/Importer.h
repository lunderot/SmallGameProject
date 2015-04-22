#ifndef IMPORTER_H
#define IMPORTER_H
#include "../../../Maya/MayaExport/CommonDeclaration.h"
#include <string>

struct VertexPositionTexCoordNormalBinormalTangent
{
	float position[3];
	float texCoord[2];
	float normal[3];
	float biNormal[3];
	float tangent[3];
};

struct BoundingSphere
{
	BoundingSphere()
	{
		position[0] = 0.0;
		position[1] = 0.0;
		position[2] = 0.0;
		radius = 0.0f;
	};
	double position[3];
	float radius;
};

struct Model
{
	Model()
	{
		MeshID = 0;
		MaterialID = 0;
		position[0] = 0.0;
		position[1] = 0.0;
		position[2] = 0.0;
		rotation[0] = 0.0f;
		rotation[1] = 0.0f;
		rotation[2] = 0.0f;
		rotation[3] = 1.0f;
		scale[0] = 1.0;
		scale[1] = 1.0;
		scale[2] = 1.0;
	};
	unsigned int MeshID;
	unsigned int MaterialID;
	double position[3];
	float rotation[4];
	double scale[3];
};

using namespace std;

class Importer
{
public:
	Importer();
	~Importer();
	
	bool importFile(string);

	unsigned int getNumMaterials() const;
	//unsigned int numCameras();
	//unsigned int numLights();
	unsigned int getNumMeshes() const;
	unsigned int getNumModels() const;
	unsigned int getMeshVertexCount(unsigned int meshID) const;
	unsigned int getNumBoundingSphere() const;
	const BoundingSphere* getBoundingSphere() const;

	//unsigned int numGroups();
	const Model* getModels() const;
	const Material* getMatrials() const;
	const VertexPositionTexCoordNormalBinormalTangent* getMesh(unsigned int meshID) const;

//Allt h�r under l�r bli private
	bool extractMainHeader(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractTransformHeader(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractMeshHeader(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractCameraHeader(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractMaterialHeader(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractLightHeader(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractJointHeader(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractNurbHeader(unsigned int& offset, char* fileData, unsigned int& fileSize);

	bool extractTransforms(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractMeshes(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractCameras(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractMaterials(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractLights(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractJoint(unsigned int& offset, char* fileData, unsigned int& fileSize);
	bool extractNurb(unsigned int& offset, char* fileData, unsigned int& fileSize);

	bool constructVerticiesAndGeometry();
	bool constructModels();
	bool constructSpere();
	VertexPositionTexCoordNormalBinormalTangent** meshGeometry;
	unsigned int numModels;
	Model* models;
	unsigned int numSpheres;
	BoundingSphere* spheres;

	Header headers;

	TransformHeader* transformHeaders;
	MeshHeader* meshHeaders;
	CameraHeader* cameraHeaders;
	MaterialHeader* materialHeaders;
	LightHeader* lightHeaders;
	JointHeader* jointHeaders;
	NurbHeader* nurbHeaders;

	Transform* transforms;
	meshStruct* meshes;
	camera* cameras;
	Material* materials;
	Light* lights;
	Joint* joints;
	Nurb* nurbs;
};

#endif