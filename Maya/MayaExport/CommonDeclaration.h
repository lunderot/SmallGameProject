#ifndef COMMONDECLARATION_H
#define COMMONDECLARATION_H
#include <vector>

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
	const char* name;
	int parentID;
	double position[3];
	double rotation[4];
	double scale[3];

	friend std::ostream& operator<<(std::ostream& out, const Transform& obj)
	{
		out << "Transform Name: " << obj.name << endl
			<< "ParentID: "<< obj.parentID << endl
			<< "Position: " << obj.position[0] << ' ' << obj.position[1] << ' ' << obj.position[3] << endl
			<< "Rotation: " << obj.rotation[0] << ' ' << obj.rotation[1] << ' ' << obj.rotation[2] << ' ' << obj.rotation[3] << endl
			<< "Scale: " << obj.scale[0] << ' ' << obj.scale[1] << ' ' << obj.scale[2] << endl;
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
	Transform transform;
	double jointOrientation[4];

	friend std::ostream& operator<<(std::ostream& out, const Joint& obj)
	{
		out << "Joint Name: " << obj.transform.name << endl
			<< "ParentID: " << obj.transform.parentID << endl
			<< "Position: " << obj.transform.position[0] << ' ' << obj.transform.position[1] << ' ' << obj.transform.position[3] << endl
			<< "Rotation: " << obj.transform.rotation[0] << ' ' << obj.transform.rotation[1] << ' ' << obj.transform.rotation[2] << ' ' << obj.transform.rotation[3] << endl
			<< "Scale: " << obj.transform.scale[0] << ' ' << obj.transform.scale[1] << ' ' << obj.transform.scale[2] << endl
			<< "Orientation: " << obj.jointOrientation[0] << ' ' << obj.jointOrientation[1] << ' ' << obj.jointOrientation[2] << endl;
		return out;
	}
};

struct MeshHeader
{
	unsigned int name_length;
	unsigned int vertex_count;
	unsigned int triangle_count;
	unsigned int joint_count;
	bool has_Animation;
};

struct Vertex
{
	//unsigned int vertexID;
	double position[3];
	float normal[3];
	double tangent[3];
	double bi_tangent[3];
	float uv[2];
};

struct meshStruct
{
	MeshHeader meshHeader;
	const char* name;
	vector <Vertex> vertices;
	vector <unsigned int> indices;
};

struct Face
{
	unsigned int vertexID[3];
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
	const char* name;
	unsigned int parentID;
	double position[3];
	double up_vector[3];
	double interest_position[3];
	double field_of_view_x;
	double field_of_view_y;
	double near_plane;
	double far_plane;
	enum projection_type{ ePerspective, eOrthogonal } projection;

	friend std::ostream& operator<<(std::ostream& out, const camera& obj)
	{
		out << "CAMERA" << endl
			<< "Name: " << obj.name << endl
			<< "ParentID: " << obj.parentID << endl
			<< "Position: " << obj.position[0] << ' ' << obj.position[1] << ' ' << obj.position[3] << endl
			<< "Up Vector: " << obj.up_vector[0] << ' ' << obj.up_vector[1] << ' ' << obj.up_vector[2] << endl
			<< "FOV X: " << obj.field_of_view_x << endl
			<< "FOV Y: " << obj.field_of_view_y << endl
			<< "Near Plane: " << obj.near_plane << endl
			<< "Far Plane: " << obj.far_plane << endl;
		return out;
	}
};

struct MorphAnimation
{
	// key frames
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
			out<< obj.normal_map;
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
	double intensity;
	enum decay_type{ eNone, eLinear, eQuadric, eCubic, noSuppDecay }dType;
	bool cast_shadows;
	double shadow_color[3];
	// posheeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeej
	// direction
	// bla bla
};

#endif