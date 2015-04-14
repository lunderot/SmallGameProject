#ifndef IMPORTER_H
#define IMPORTER_H
#include "../../../Maya/MayaExport/CommonDeclaration.h"
#include <string>
#include <vector>

using namespace std;
class Importer
{
public:
	Importer();
	~Importer();

	bool importFile(string);

	Header headers;

	vector<TransformHeader> transformHeaders;
	vector<MeshHeader> meshHeaders;
	vector<CameraHeader> cameraHeaders;
	vector<MaterialHeader> materialHeaders;
	vector<LightHeader> lightHeaders;

	vector<Transform> transforms;
	vector<meshStruct> meshes;
	vector<camera> cameras;
	vector<Material> materials;
	vector<Light> lights;
};

#endif