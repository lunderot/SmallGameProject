#ifndef COMMONDECLARATION_H
#define COMMONDECLARATION_H

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
		out << "Transform name length: " <<obj.name_Length;
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
		out << "Name: " << obj.name << endl
			<< "ParentID: "<< obj.parentID << endl
			<< "Position: " << obj.position[0] << ' ' << obj.position[1] << ' ' << obj.position[3] << endl
			<< "Rotation: " << obj.rotation[0] << ' ' << obj.rotation[1] << ' ' << obj.rotation[2] << ' ' << obj.rotation[3] << endl
			<< "Scale: " <<obj.scale[0] << ' ' << obj.scale[1] << ' ' << obj.scale[2];
		return out;
	}
};

struct MeshHeader
{
	unsigned int name_length;
	unsigned int vertex_count;
	unsigned int joint_count;
	bool has_Animation;
};

struct Vertex
{
	unsigned int vertexID;
	double position[3];
	double normal[3];
	double tangent[3];
	double bi_tangent[3];
	double uv[2];
};

struct CameraHeader
{
	unsigned int name_length;
};

struct camera
{
	//char name[];
	double position[3];
	double up_vector[3];
	double interest_position[3];
	double roll[3];
	double field_of_view;
	double field_of_view_x;
	double field_of_view_y;
	double fear_plane;
	double far_plane;
	enum projection_type{ ePerspective, eOrthogonal };
};

struct MorphAnimation
{
	// key frames
	// vertex
	// color
};

struct MaterialHeader
{
	unsigned int name_length;
	unsigned int duffuse_map_length;
	unsigned int normal_map_length;
	unsigned int specular_map_length;
};

struct Material
{
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
	enum light_type{ ePoint, eDirectional, eSpot, eArea, eVolume };
	double color[3];
	double intensity;
	enum decay_type{ eNone, eLinear, eQuadric, eCubic };
	bool cast_shadows;
	double shadow_color[3];
	// posheeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeej
	// direction
	// bla bla
};

#endif