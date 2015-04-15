#ifndef COMMONDECLARATION_H
#define COMMONDECLARATION_H
#include <vector>
#include <fstream>

using namespace std;

struct Header
{
	Header()
	{
		group_count = mesh_count = material_count = camera_count = light_count = 0;
	};

	unsigned int group_count;
	unsigned int mesh_count;
	unsigned int material_count;
	unsigned int camera_count;
	unsigned int light_count;

	friend std::ostream& operator<<(std::ostream& out, const Header& obj)
	{
		out << "Number of groups: " << obj.group_count << endl
			<< "Number of meshes: " << obj.mesh_count << endl
			<< "Number of materials: " << obj.material_count << endl
			<< "Number of cameras: " << obj.camera_count << endl
			<< "Number of lights: " << obj.light_count << endl;
		return out;
	}
};

struct TransformHeader
{
	unsigned int name_Length;

	friend std::ostream& operator<<(std::ostream& out, const TransformHeader& obj)
	{
		out << "Transform name length: " << obj.name_Length << endl;
		return out;
	}
};

struct Transform
{
	int parentID;
	double position[3];
	double rotation[4];
	double scale[3];
	const char* name;

	void WriteBinary(TransformHeader* header, ofstream& outputfile)
	{
		char* output = (char*) this;
		outputfile.write(output, sizeof(Transform) - sizeof(const char*));
		outputfile.write(name, sizeof(const char) * header->name_Length);
	}

	friend std::ostream& operator<<(std::ostream& out, const Transform& obj)
	{
		out << "ParentID: " << obj.parentID << endl
			<< "Position: " << obj.position[0] << ' ' << obj.position[1] << ' ' << obj.position[3] << endl
			<< "Rotation: " << obj.rotation[0] << ' ' << obj.rotation[1] << ' ' << obj.rotation[2] << ' ' << obj.rotation[3] << endl
			<< "Scale: " << obj.scale[0] << ' ' << obj.scale[1] << ' ' << obj.scale[2] << endl
			<< "Transform Name: " << obj.name << endl;
		return out;
	}
};

struct JointHeader
{
	TransformHeader	transformHeader;

	friend std::ostream& operator<<(std::ostream& out, const JointHeader& obj)
	{
		out << "Joint name length: " << obj.transformHeader.name_Length << endl;
		return out;
	}
};

struct Joint
{
	double jointOrientation[4];
	Transform transform;

	void WriteBinary(JointHeader* header, ofstream& outputfile)
	{
		char* output = (char*) this;
		outputfile.write((const char*)output, sizeof(Joint) - sizeof(const char*));
		transform.WriteBinary(&header->transformHeader, outputfile);
	}

	friend std::ostream& operator<<(std::ostream& out, const Joint& obj)
	{
		out << "ParentID: " << obj.transform.parentID << endl
			<< "Position: " << obj.transform.position[0] << ' ' << obj.transform.position[1] << ' ' << obj.transform.position[3] << endl
			<< "Rotation: " << obj.transform.rotation[0] << ' ' << obj.transform.rotation[1] << ' ' << obj.transform.rotation[2] << ' ' << obj.transform.rotation[3] << endl
			<< "Scale: " << obj.transform.scale[0] << ' ' << obj.transform.scale[1] << ' ' << obj.transform.scale[2] << endl
			<< "Orientation: " << obj.jointOrientation[0] << ' ' << obj.jointOrientation[1] << ' ' << obj.jointOrientation[2] << endl
			<< "Joint Name: " << obj.transform.name << endl;
		return out;
	}
};

struct MeshHeader
{
	unsigned int name_length;
	unsigned int vertex_count;
	unsigned int indice_count;
	unsigned int position_count;
	unsigned int uv_count;
	unsigned int normal_count;
	unsigned int tangent_count;
	unsigned int biTangent_count;
	unsigned int material_count;
	unsigned int transform_count;
	unsigned int joint_count;
	bool has_Animation;

	friend std::ostream& operator<<(std::ostream& out, const MeshHeader& obj)
	{
		out << "Name Length: " << obj.name_length << endl
			<< "Vertex Count: " << obj.vertex_count << endl
			<< "Indices Count: " << obj.indice_count << endl
			<< "Position Count: " << obj.position_count << endl
			<< "Uv Count: " << obj.uv_count << endl
			<< "Normal Count: " << obj.normal_count << endl
			<< "tangent Count: " << obj.tangent_count << endl
			<< "bi-Tangent Count: " << obj.biTangent_count << endl
			<< "Material Count: " << obj.material_count << endl
			<< "Transform Count: " << obj.transform_count << endl;
		return out;
	}
};

struct Vertex
{
	unsigned int position;
	unsigned int uv;
	unsigned int normal;

	void WriteBinary(ofstream& outputfile)
	{
		outputfile.write((const char*) this, sizeof(Vertex));
	}

	friend std::ostream& operator<<(std::ostream& out, const Vertex& obj)
	{
		out	<< obj.position << 
			" / " << obj.uv <<
			" / " << obj.normal;
		return out;
	}
};

struct meshStruct
{
	vector <vector<double>> position;
	vector <vector<float>> uv;
	vector <vector<double>> normal;
	vector <vector<double>> tangent;
	vector <vector<double>> bi_tangent;

	vector <unsigned int> transform_Id;
	vector <unsigned int> material_Id;
	vector <Vertex> vertices;
	const char* name;

	void WriteBinary(MeshHeader* header, ofstream& outputfile)
	{
		char* output = (char*) this;
		outputfile.write((const char*)output, sizeof(meshStruct) - sizeof(const char*));
		outputfile.write(name, sizeof(char) * header->name_length);
	}

	friend std::ostream& operator<<(std::ostream& out, const meshStruct& obj)
	{
		out << "Mesh Name: " << obj.name << endl
			<< "Transform id: " << endl;
		for (unsigned int i = 0; i < obj.transform_Id.size(); i++)
		{
			out << obj.transform_Id[i] << endl;
		}

		out << "Material id: " << endl;

		for (unsigned int i = 0; i < obj.material_Id.size(); i++)
		{
			out << obj.material_Id[i] << endl;
		}

		for (unsigned int i = 0; i < obj.position.size(); i++)
		{
			out << "Position " << i << ": " << obj.position[i][0] << " " << obj.position[i][1] << " " << obj.position[i][2] << endl;
		}
		for (unsigned int i = 0; i < obj.uv.size(); i++)
		{
			out << "UV " << i << ": " << obj.uv[i][0] << " " << obj.uv[i][1] << endl;
		}
		for (unsigned int i = 0; i < obj.normal.size(); i++)
		{
			out << "Normal " << i << ": " << obj.normal[i][0] << " " << obj.normal[i][1] << " " << obj.normal[i][2] << endl;
		}
		for (unsigned int i = 0; i < obj.tangent.size(); i++)
		{
			out << "Tangent " << i << ": " << obj.tangent[i][0] << " " << obj.tangent[i][1] << " " << obj.tangent[i][2] << endl;
		}
		for (unsigned int i = 0; i < obj.bi_tangent.size(); i++)
		{
			out << "Bi-Tangent " << i << ": " << obj.bi_tangent[i][0] << " " << obj.bi_tangent[i][1] << " " << obj.bi_tangent[i][2] << endl;
		}

		int tmp = 0;
		int tmp_faceCounter = 0;
		for (unsigned int i = 0; i < obj.vertices.size(); i++)
		{
			if (tmp == 0 || tmp == 3)
			{
				out << "Face " <<  tmp_faceCounter << ": " << endl;
				tmp_faceCounter++;
				tmp = 0;
			}
			out << obj.vertices[i] << endl;
			tmp++;
		}

		return out;
	}
};

struct CameraHeader
{
	unsigned int name_length;

	friend std::ostream& operator<<(std::ostream& out, const CameraHeader& obj)
	{
		out << "Camera name length: " << obj.name_length << endl;
		return out;
	}
};

struct camera
{
	unsigned int parentID;
	double position[3];
	double up_vector[3];
	double interest_position[3];
	double field_of_view_x;
	double field_of_view_y;
	double near_plane;
	double far_plane;
	enum projection_type{ ePerspective, eOrthogonal } projection;
	const char* name;

	void WriteBinary(CameraHeader* header, ofstream& outputfile)
	{
		//MGlobal::displayInfo(MString() + header->name_length);
		char* output = (char*) this;
		outputfile.write((const char*)output, sizeof(camera) - sizeof(const char*));
		outputfile.write(name, header->name_length);
	}

	friend std::ostream& operator<<(std::ostream& out, const camera& obj)
	{
		out << "CAMERA" << endl
			<< "ParentID: " << obj.parentID << endl
			<< "Position: " << obj.position[0] << ' ' << obj.position[1] << ' ' << obj.position[3] << endl
			<< "Up Vector: " << obj.up_vector[0] << ' ' << obj.up_vector[1] << ' ' << obj.up_vector[2] << endl
			<< "Intrest position: " << obj.interest_position[0] << ' ' << obj.interest_position[1] << ' ' << obj.interest_position[2] << endl
			<< "FOV X: " << obj.field_of_view_x << endl
			<< "FOV Y: " << obj.field_of_view_y << endl
			<< "Near Plane: " << obj.near_plane << endl
			<< "Far Plane: " << obj.far_plane << endl
			<< "Projection type: " << obj.projection << endl
			<< "Name: " << obj.name << endl;
		return out;
	}
};
struct morphAnimationHeader
{
	unsigned int base_name_length;
	unsigned int blendShape_name_length;

	friend std::ostream& operator<<(std::ostream& out, const morphAnimationHeader& obj)
	{
		out << "base name length: " << obj.base_name_length << endl
			<< "Blend Shape name length: " << obj.blendShape_name_length << endl;
		return out;
	}
};
struct MorphAnimation
{
	unsigned int nrOfWeights;
	unsigned int nrOfTargets;
	unsigned int nrOfVertsPerMesh;
	vector <vector<double>> position;
	const char* baseName;
	const char* blendShapeName;

	void WriteBinary(morphAnimationHeader* header, ofstream& outputfile)
	{
		//MGlobal::displayInfo(MString() + header->name_length);
		char* output = (char*) this;
		outputfile.write((const char*)output, sizeof(MorphAnimation)-sizeof(const char*));
		outputfile.write(baseName, header->base_name_length);
		outputfile.write(blendShapeName, header->blendShape_name_length);
	}

	friend std::ostream& operator<<(std::ostream& out, const MorphAnimation& obj)
	{
		out << "nrOfWeights: " << obj.nrOfWeights << endl
			<< "nrOfTargets: " << obj.nrOfTargets << endl
			<< "nrOfVertsPerMesh: " << obj.nrOfVertsPerMesh << endl;
		for (unsigned int i = 0; i < obj.position.size(); i++)
		{
			out << "Position:" << obj.position[i][0] << "/" << obj.position[i][1] << "/" << obj.position[i][2] << endl;
		}
		out << "Base Name:" << obj.baseName << endl
			<< "blendShapeName" << obj.blendShapeName << endl;
		return out;
	}
	// vertex
	// color
};

struct MaterialHeader
{
	MaterialHeader()
	{
		name_length = duffuse_map_length = normal_map_length = specular_map_length = 0;
	}
	unsigned int name_length;
	unsigned int duffuse_map_length;
	unsigned int normal_map_length;
	unsigned int specular_map_length;

	friend std::ostream& operator<<(std::ostream& out, const MaterialHeader& obj)
	{
		out << "Material name lenght: " << obj.name_length << endl
			<< "Diffuse map name lenght: " << obj.duffuse_map_length << endl
			<< "Normal map name lenght: " << obj.normal_map_length << endl
			<< "Specular map name lenght: " << obj.specular_map_length << endl;
		return out;
	}
};

struct Material
{
	Material()
	{
		mtrl_type = eLambert;
		normal_depth = specular_factor = shininess = reflection_factor = diffuse_factor = 0;
		specular[3] = reflection[3] = ambient[3] = diffuse[3] = transparency_color[3] = incandescence[3] = { 0.0f };
		node_name = nullptr;
		diffuse_map = nullptr;
		normal_map = nullptr;
		specular_map = nullptr;
	}
	enum material_type { eLambert, eBlinn, ePhong } mtrl_type;
	double normal_depth;
	double specular[3];
	double specular_factor;
	double shininess;
	double reflection[3];
	double reflection_factor;
	double ambient[3];
	double diffuse[3];
	double diffuse_factor;
	double transparency_color[3];
	double incandescence[3];
	const char* node_name;
	const char* diffuse_map;
	const char* normal_map;
	const char* specular_map;

	void WriteBinary(MaterialHeader* header, ofstream& outputfile)
	{
		char* output = (char*) this;
		outputfile.write((const char*)output, sizeof(Material) - sizeof(const char*) * 4);
		outputfile.write(node_name, sizeof(char) * header->name_length);
		outputfile.write(diffuse_map, sizeof(char) * header->duffuse_map_length);
		outputfile.write(normal_map, sizeof(char) * header->normal_map_length);
		outputfile.write(specular_map, sizeof(char) * header->specular_map_length);
	}

	friend std::ostream& operator<<(std::ostream& out, const Material& obj)
	{
		out << "Material type: " << obj.mtrl_type << endl
			<< "Normal depth: " << obj.normal_depth << endl
			<< "Specular: " << obj.specular[0] << " " << obj.specular[1] << " " << obj.specular[2] << endl
			<< "Specular factor: " << obj.specular_factor << endl
			<< "Shininess: " << obj.shininess << endl
			<< "Reflection: " << obj.reflection[0] << " " << obj.reflection[1] << " " << obj.reflection[2] << endl
			<< "Reflection factor: " << obj.reflection_factor << endl
			<< "Ambient: " << obj.ambient[0] << " " << obj.ambient[1] << " " << obj.ambient[2] << endl
			<< "Diffuse: " << obj.diffuse[0] << " " << obj.diffuse[1] << " " << obj.diffuse[2] << endl
			<< "Diffuse factor: " << obj.diffuse_factor << endl
			<< "Transparency color: " << obj.transparency_color[0] << " " << obj.transparency_color[1] << " " << obj.transparency_color[2] << endl
			<< "Incandescence: " << obj.incandescence[0] << " " << obj.incandescence[1] << " " << obj.incandescence[2] << endl
			<< "Name: " << obj.node_name << endl
			<< "Diffuse map: ";
		if (obj.diffuse_map != nullptr)
			out << obj.diffuse_map;
		out << endl
			<< "Normal map: ";
		if (obj.diffuse_map != nullptr)
			out << obj.normal_map;
		out << endl
			<< "Specular map: ";
		if (obj.diffuse_map != nullptr)
			out << obj.specular_map;
		out << endl;
		return out;
	}
};

struct Animation
{
	unsigned int joint_ID[4];
	double joint_weight[4];
	// bone-id
	// weight
	// bla bla
};

struct LightHeader
{
	unsigned int name_Lenght;
};

struct Light
{
	//char name[];
	enum light_type{ ePoint, eDirectional, eSpot, eArea, eVolume }type;
	double color[3];
	float intensity;
	enum decay_type{ eNone, eLinear, eQuadric, eCubic }dType;
	//short decay_type;
	bool cast_shadows;
	double shadow_color[3];
	// posheeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeej
	// direction
	// bla bla
};

#endif