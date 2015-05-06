#include "material.h"
#include <Windows.h>

using namespace std;

MStatus Materials::exportMaterial(vector<MaterialData>& mat_vector, map<const char*, int>& mat_map, std::string output_dir, vector<const char*> &node_name, vector<const char*> &diffuse_map, vector<const char*> &normal_map, vector<const char*> &specular_map)
{
	MStatus status = MStatus::kSuccess;

	this->outputDir = output_dir;

	MItDependencyNodes matIt(MFn::kLambert);

	MaterialData mat_struct;

	while (!matIt.isDone())
	{
		if (matIt.thisNode().hasFn(MFn::kPhong))
		{
			MFnDependencyNode materialFn(matIt.thisNode());

			mat_map[materialFn.name().asChar()] = mat_vector.size();

			mat_struct.mtrl_type = material_type::ePhong;

			this->commonDiffuseValues(materialFn, mat_struct, node_name, diffuse_map, normal_map, specular_map);
			this->commonReflectValues(materialFn, mat_struct, node_name, diffuse_map, normal_map, specular_map);

			plug = materialFn.findPlug("cosinePower");
			plug.getValue(mat_struct.specular_factor);

			mat_struct.shininess = 0.0f;

			this->findTextures(materialFn, mat_struct, node_name, diffuse_map, normal_map, specular_map);
			if (mat_struct.duffuse_map_length == 0)
			{
				diffuse_map.push_back(nullptr);
			}
			if (mat_struct.normal_map_length == 0)
			{
				normal_map.push_back(nullptr);
			}
			if (mat_struct.specular_map_length == 0)
			{
				specular_map.push_back(nullptr);
			}
		}
		else if (matIt.thisNode().hasFn(MFn::kBlinn))
		{
			MFnDependencyNode materialFn(matIt.thisNode());

			mat_map[materialFn.name().asChar()] = mat_vector.size();

			mat_struct.mtrl_type = material_type::eBlinn;

			this->commonDiffuseValues(materialFn, mat_struct, node_name, diffuse_map, normal_map, specular_map);
			this->commonReflectValues(materialFn, mat_struct, node_name, diffuse_map, normal_map, specular_map);

			plug = materialFn.findPlug("eccentricity");
			plug.getValue(mat_struct.specular_factor);

			plug = materialFn.findPlug("specularRollOff");
			plug.getValue(mat_struct.shininess);

			this->findTextures(materialFn, mat_struct, node_name, diffuse_map, normal_map, specular_map);
			if (mat_struct.duffuse_map_length == 0)
			{
				diffuse_map.push_back(nullptr);
			}
			if (mat_struct.normal_map_length == 0)
			{
				normal_map.push_back(nullptr);
			}
			if (mat_struct.specular_map_length == 0)
			{
				specular_map.push_back(nullptr);
			}

		}
		else if (matIt.thisNode().hasFn(MFn::kLambert))
		{
			MFnDependencyNode materialFn(matIt.thisNode());

			mat_map[materialFn.name().asChar()] = mat_vector.size();

			mat_struct.mtrl_type = material_type::eLambert;

			this->commonDiffuseValues(materialFn, mat_struct, node_name, diffuse_map, normal_map, specular_map);

			mat_struct.shininess = 0.0f;
			mat_struct.specular[0] = 0.0f;
			mat_struct.specular[1] = 0.0f;
			mat_struct.specular[2] = 0.0f;
			mat_struct.specular_factor = 0.0f;
			mat_struct.reflection[0] = 0.0f;
			mat_struct.reflection[1] = 0.0f;
			mat_struct.reflection[2] = 0.0f;
			mat_struct.reflection_factor = 0.0f;

			this->findTextures(materialFn, mat_struct, node_name, diffuse_map, normal_map, specular_map);
			if (mat_struct.duffuse_map_length == 0)
			{
				diffuse_map.push_back(nullptr);
			}
			if (mat_struct.normal_map_length == 0)
			{
				normal_map.push_back(nullptr);
			}
			if (mat_struct.specular_map_length == 0)
			{
				specular_map.push_back(nullptr);
			}
		}

		// Write to file or push_back the data here
		mat_vector.push_back(mat_struct);

		matIt.next();
	}

	return status;
}

MStatus Materials::commonDiffuseValues(MFnDependencyNode& node, MaterialData& matStrct, vector<const char*> &node_name, vector<const char*> &diffuse_map, vector<const char*> &normal_map, vector<const char*> &specular_map)
{
	//matStrct.node_name = node.name().asChar();
	node_name.push_back(node.name().asChar());
	matStrct.name_length = node.name().length();

	plug = node.findPlug("colorR");
	plug.getValue(matStrct.diffuse[0]);
	plug = node.findPlug("colorG");
	plug.getValue(matStrct.diffuse[1]);
	plug = node.findPlug("colorB");
	plug.getValue(matStrct.diffuse[2]);
	plug = node.findPlug("diffuse");
	plug.getValue(matStrct.diffuse_factor);

	plug = node.findPlug("transparencyR");
	plug.getValue(matStrct.transparency_color[0]);
	plug = node.findPlug("transparencyG");
	plug.getValue(matStrct.transparency_color[1]);
	plug = node.findPlug("transparencyB");
	plug.getValue(matStrct.transparency_color[2]);

	plug = node.findPlug("ambientColorR");
	plug.getValue(matStrct.ambient[0]);
	plug = node.findPlug("ambientColorG");
	plug.getValue(matStrct.ambient[1]);
	plug = node.findPlug("ambientColorB");
	plug.getValue(matStrct.ambient[2]);

	plug = node.findPlug("incandescenceR");
	plug.getValue(matStrct.incandescence[0]);
	plug = node.findPlug("incandescenceG");
	plug.getValue(matStrct.incandescence[1]);
	plug = node.findPlug("incandescenceB");
	plug.getValue(matStrct.incandescence[2]);

	return MStatus::kSuccess;
}

MStatus Materials::commonReflectValues(MFnDependencyNode& node, struct MaterialData& matStrct, vector<const char*> &node_name, vector<const char*> &diffuse_map, vector<const char*> &normal_map, vector<const char*> &specular_map)
{
	plug = node.findPlug("specularColorR");
	plug.getValue(matStrct.specular[0]);
	plug = node.findPlug("specularColorG");
	plug.getValue(matStrct.specular[1]);
	plug = node.findPlug("specularColorB");
	plug.getValue(matStrct.specular[2]);

	plug = node.findPlug("reflectedColorR");
	plug.getValue(matStrct.reflection[0]);
	plug = node.findPlug("reflectedColorG");
	plug.getValue(matStrct.reflection[1]);
	plug = node.findPlug("reflectedColorB");
	plug.getValue(matStrct.reflection[2]);
	plug = node.findPlug("reflectivity");
	plug.getValue(matStrct.reflection_factor);

	return MStatus::kSuccess;
}

MStatus Materials::findTextures(MFnDependencyNode& node, MaterialData& matStrct, vector<const char*> &node_name, vector<const char*> &diffuse_map, vector<const char*> &normal_map, vector<const char*> &specular_map)
{
	bool asDst = true, asSrc = false;
	MPlugArray allConnections;
	MObject object;

	plug = node.findPlug("color", true);
	plug.connectedTo(allConnections, asDst, asSrc);

	for (unsigned int i = 0; i < allConnections.length(); i++)
	{
		object = allConnections[i].node();
		if (object.hasFn(MFn::kFileTexture))
		{
			MFnDependencyNode texture_node(object);
			MPlug filename = texture_node.findPlug("ftn");

			string temp = filename.asString().asChar();
			int lastSlash = temp.find_last_of("/");
			string fileName = temp.substr(lastSlash + 1);

			matStrct.duffuse_map_length = fileName.size();

			string tempOut = this->outputDir;
			tempOut.append("/");
			tempOut.append(fileName);

			//matStrct.diffuse_map = new char[fileName.size() + 1];
			//for (unsigned int x = 0; x < matStrct.duffuse_map_length; x++)
			//{
			//	matStrct.diffuse_map[x] = fileName[x];
			//}
			//matStrct.diffuse_map[matStrct.duffuse_map_length] = '\0';

			char* diffuse_map_temp = new char[fileName.size() + 1];
			for (unsigned int x = 0; x < matStrct.duffuse_map_length; x++)
			{
				diffuse_map_temp[x] = fileName[x];
			}
			diffuse_map_temp[matStrct.duffuse_map_length] = '\0';

			diffuse_map.push_back((const char*)diffuse_map_temp);
			
			CopyFile(temp.c_str(), tempOut.c_str(), TRUE);

			break;
		}
	}

	if (matStrct.mtrl_type != material_type::eLambert)
	{
		plug = node.findPlug("specularColor", true);
		plug.connectedTo(allConnections, asDst, asSrc);

		for (unsigned int i = 0; i < allConnections.length(); i++)
		{
			object = allConnections[i].node();
			if (object.hasFn(MFn::kFileTexture))
			{
				MFnDependencyNode texture_node(object);
				MPlug filename = texture_node.findPlug("ftn");

				string temp = filename.asString().asChar();
				int lastSlash = temp.find_last_of("/");
				string fileName = temp.substr(lastSlash + 1);

				matStrct.specular_map_length = fileName.size();

				string tempOut = this->outputDir;
				tempOut.append("/");
				tempOut.append(fileName);

				//matStrct.specular_map = new char[fileName.size() + 1];
				//for (unsigned int x = 0; x < matStrct.specular_map_length; x++)
				//{
				//	matStrct.specular_map[x] = fileName[x];
				//}
				//matStrct.specular_map[matStrct.specular_map_length] = '\0';

				char* specular_map_temp = new char[fileName.size() + 1];
				for (unsigned int x = 0; x < matStrct.specular_map_length; x++)
				{
					specular_map_temp[x] = fileName[x];
				}
				specular_map_temp[matStrct.specular_map_length] = '\0';

				specular_map.push_back((const char*)specular_map_temp);

				CopyFile(temp.c_str(), tempOut.c_str(), TRUE);

				break;
			}
		}
	}

	plug = node.findPlug("normalCamera", true);
	plug.connectedTo(allConnections, asDst, asSrc);

	for (unsigned int i = 0; i < allConnections.length(); i++)
	{
		if (allConnections[i].node().apiType() == MFn::kBump)
		{
			MFnDependencyNode fnBump(allConnections[i].node());
			MPlug bumpDepth = fnBump.findPlug("bumpDepth");

			bumpDepth.getValue(matStrct.normal_depth);

			MPlug bump_value = fnBump.findPlug("bumpValue");
			MPlugArray bumpValueConnections;
			MObject bumpValueObject;

			bump_value.connectedTo(bumpValueConnections, asDst, asSrc);
			for (unsigned int j = 0; j < bumpValueConnections.length(); j++)
			{
				bumpValueObject = bumpValueConnections[i].node();
				if (bumpValueObject.hasFn(MFn::kFileTexture))
				{
					MFnDependencyNode texture_node(bumpValueObject);
					MPlug filename = texture_node.findPlug("ftn");

					string temp = filename.asString().asChar();
					int lastSlash = temp.find_last_of("/");
					string fileName = temp.substr(lastSlash + 1);

					matStrct.normal_map_length = fileName.size();

					string tempOut = this->outputDir;
					tempOut.append("/");
					tempOut.append(fileName);

					//matStrct.normal_map = new char[fileName.size() + 1];
					//for (unsigned int x = 0; x < matStrct.normal_map_length; x++)
					//{
					//	matStrct.normal_map[x] = fileName[x];
					//}
					//matStrct.normal_map[matStrct.normal_map_length] = '\0';

					char* normal_map_temp = new char[fileName.size() + 1];
					for (unsigned int x = 0; x < matStrct.normal_map_length; x++)
					{
						normal_map_temp[x] = fileName[x];
					}
					normal_map_temp[matStrct.normal_map_length] = '\0';

					normal_map.push_back((const char*)normal_map_temp);

					CopyFile(temp.c_str(), tempOut.c_str(), TRUE);

					break;
				}
			}
			break;
		}
	}

	return MStatus::kSuccess;
}
