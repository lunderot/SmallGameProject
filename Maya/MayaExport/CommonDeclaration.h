#ifndef COMMONDECLARATION_H
#define COMMONDECLARATION_H

struct Header
{
	unsigned int group_count;
	unsigned int mesh_count;
	unsigned int material_count;
	unsigned int camera_count;
};

struct TransformHeader
{
	unsigned int name_Length;
};

struct Transform
{	
	//char name[];
	double position[3];
	double rotation[4];
	double scale[3];
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

struct shapeHeader
{
	unsigned int name_length;
};

struct shape
{
	//char name[];
	unsigned int mesh_ID;
	unsigned int transform_ID;
	unsigned int material_ID;
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
	unsigned int name_Length;
};

struct Material
{
	//char name[];
	bool is_phong;
	double specular[3];
	double specular_factor;
	double shininess;
	double reflection[3];
	double reflection_factor;

	// id
	double ambient[3];
	double ambient_factor;
	double diffuse[3];
	double diffuse_factor;
	double normal_map[3];
	double transparent_color[3];
	double transparency_factor;

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