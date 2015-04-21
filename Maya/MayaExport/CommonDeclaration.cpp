#include "CommonDeclaration.h"

//Header
std::ostream& operator<<(std::ostream& out, const Header& obj)
{
	out << "Number of groups: " << obj.group_count << endl
		<< "Number of meshes: " << obj.mesh_count << endl
		<< "Number of materials: " << obj.material_count << endl
		<< "Number of cameras: " << obj.camera_count << endl
		<< "Number of lights: " << obj.light_count << endl
		<< "Number of joints: " << obj.joint_count << endl;
	return out;
}

//TransformHeader
std::ostream& operator<<(std::ostream& out, const TransformHeader& obj)
{
	out << "Transform name length: " << obj.name_Length << endl;
	return out;
}

//Transform
void Transform::WriteBinary(TransformHeader* header, ofstream& outputfile)
{
	char* output = (char*) this;
	outputfile.write(output, sizeof(Transform) - sizeof(const char*));
	outputfile.write(name, sizeof(const char) * header->name_Length);
}

std::ostream& operator<<(std::ostream& out, const Transform& obj)
{
	out << "ParentID: " << obj.parentID << endl
		<< "Position: " << obj.position[0] << ' ' << obj.position[1] << ' ' << obj.position[3] << endl
		<< "Rotation: " << obj.rotation[0] << ' ' << obj.rotation[1] << ' ' << obj.rotation[2] << ' ' << obj.rotation[3] << endl
		<< "Scale: " << obj.scale[0] << ' ' << obj.scale[1] << ' ' << obj.scale[2] << endl
		<< "Transform Name: " << obj.name << endl;
	return out;
}

//CameraHeader
std::ostream& operator<<(std::ostream& out, const CameraHeader& obj)
{
	out << "Camera name length: " << obj.name_length << endl
		<< "Number of parents: " << obj.nrOfParents << endl;
	return out;
}

//Camera
void camera::WriteBinary(CameraHeader* header, ofstream& outputfile)
{
	//MGlobal::displayInfo(MString() + header->name_length);
	char* output = (char*) this;
	outputfile.write((const char*)output, sizeof(camera) - sizeof(const char*) - sizeof(unsigned int*));
	outputfile.write((const char*)parentID, sizeof(int) * header->nrOfParents);
	outputfile.write(name, header->name_length);
}

std::ostream& operator<<(std::ostream& out, const camera& obj)
{
	out << "CAMERA" << endl;
	out	<< "ParentID: " << obj.parentID << endl;
	out	<< "Position: " << obj.position[0] << ' ' << obj.position[1] << ' ' << obj.position[3] << endl
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

//MaterialHeader
std::ostream& operator<<(std::ostream& out, const MaterialHeader& obj)
{
	out << "Material name lenght: " << obj.name_length << endl
		<< "Diffuse map name lenght: " << obj.duffuse_map_length << endl
		<< "Normal map name lenght: " << obj.normal_map_length << endl
		<< "Specular map name lenght: " << obj.specular_map_length << endl;
	return out;
}

//Mateial
void Material::WriteBinary(MaterialHeader* header, ofstream& outputfile)
{
	char* output = (char*) this;
	outputfile.write((const char*)output, sizeof(Material) - sizeof(const char*) * 4);
	outputfile.write(node_name, sizeof(char) * header->name_length);
	outputfile.write(diffuse_map, sizeof(char) * header->duffuse_map_length);
	outputfile.write(normal_map, sizeof(char) * header->normal_map_length);
	outputfile.write(specular_map, sizeof(char) * header->specular_map_length);
}

std::ostream& operator<<(std::ostream& out, const Material& obj)
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


// LightHeader
std::ostream& operator<<(std::ostream& out, const LightHeader& obj)
{
	out << "Light name length: " << obj.name_Length<< endl;
	return out;
}

// Light 
void Light::WriteBinary(LightHeader* header, ofstream& outputfile)
{
	char* output = (char*) this;
	outputfile.write(output, sizeof(Light)-sizeof(const char*));
	outputfile.write(name, header->name_Length);
}

std::ostream& operator<<(std::ostream& out, const Light& obj)
{
	out << "LIGHT" << endl
		<< "Name of light: " << obj.name << endl
		<< "Light type: " << obj.type << endl
		<< "Decay type: " << obj.dType << endl
		<< "Color: " << obj.color[0] << " " << obj.color[1] << " " << obj.color[2] << endl
		<< "Intensity: " << obj.intensity << endl
		<< "Cast shadow: " << obj.cast_shadows << endl
		<< "Shadow color: " << obj.shadow_color[0] << " " << obj.shadow_color[1] << " " << obj.shadow_color[2] << endl;
	return out;
}

// NurbHeader
std::ostream& operator<<(std::ostream& out, const NurbHeader& obj)
{
	out << " Nurb name lengtht: " << obj.name_Length << endl
		<< " Number of nurbs: " << obj.numberOfParent << endl;
	return out;
}

// Nurb
void Nurb::WriteBinary(NurbHeader* header, ofstream& outputfile)
{
	char* output = (char*) this;
	outputfile.write(output, sizeof(Nurb)-sizeof(const char*));
	outputfile.write(name, header->name_Length);
}

// Nurb
std::ostream& operator << (std::ostream& out, const Nurb& obj)
{
	out << "NURB" << endl
		<< "Name of nurb: " << obj.name << endl
		<< "Radius: " << obj.radius << endl;
	for (unsigned int i = 0; i < obj.parentID.size(); i++)
	{		
		out << "Parent ID: " << obj.parentID[i] << endl;
	}

	return out;

}

//struct MorphAnimation

void MorphAnimation::WriteBinary(morphAnimationHeader* header, ofstream& outputfile)
{
	//char* output = (char*) this;
	//outputfile.write((const char*)output, sizeof(meshStruct) - sizeof(const char*));
	for (unsigned int i = 0; i < header->nrOfVertsPerMesh * header->nrOfWeights; i++)
		outputfile.write((char*)position[i].data(), sizeof(double)* 3);

	outputfile.write(blendShapeName, header->blendShape_name_length);
}

std::ostream& operator<<(std::ostream& out, const MorphAnimation& obj)
{
	for (unsigned int i = 0; i < obj.position.size(); i++)
	{
		out << "Position:" << obj.position[i][0] << "/" << obj.position[i][1] << "/" << obj.position[i][2] << endl;
	}
	out << "blendShapeName" << obj.blendShapeName << endl
		<< "Mesh ID: " << obj.meshID << endl;

	return out;
}