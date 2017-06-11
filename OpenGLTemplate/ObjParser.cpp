#include "ObjParser.h"

//for split string
template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

o_data::o_data() : o_material(NULL)
{
	this->name = "unnamed";
}
o_data::o_data(std::string o_name) : o_material(NULL)
{
	this->name = o_name;
}

o_data::o_data(const o_data& o): o_material(NULL)
{
	name = o.name;
	verticesIndices = o.verticesIndices;
	normalsIndices = o.normalsIndices;
	texCoordsIndices = o.texCoordsIndices;
	if (o.o_material) {
		o_material = new material(*o.o_material);
	}
}

o_data::~o_data()
{
	if (o_material != NULL) delete o_material;
}

material::material(std::string name)
{
	this->name = name;
	Ka = glm::vec3(0.0);
	Kd = glm::vec3(0.0);
	Ks = glm::vec3(0.0);
	Ns = 0.0;
	d = 0.0;
	int illum = -1;
}

ObjParser::ObjParser() 
{
	currentMeshIdx = -1;
}

void ObjParser::parse(std::string filepath)
{
	directory = ObjParser::getDirectory(filepath);
	std::ifstream obj_file(filepath);
	if (!obj_file.is_open()) {
		throw FileNotFoundException(filepath);
	}

	vertices.clear();
	normals.clear();
	texCoords.clear();
	meshes.clear();

	std::map<std::string, material*> materials;
	L_INFO("Parsing obj " + filepath);
	std::string line;
	while (std::getline(obj_file, line)) {
		if (line.substr(0, 2) == "v ") {
			std::istringstream str(line.substr(2));
			glm::vec4 vec; 
			str >> vec.x; str >> vec.y; str >> vec.z; vec.w = 1.0f;
			vertices.push_back(vec);
		}
		else if (line.substr(0, 3) == "vn ") {
			std::istringstream str(line.substr(3));
			glm::vec3 vec;
			str >> vec.x; str >> vec.y; str >> vec.z;
			vec = glm::normalize(vec);
			normals.push_back(vec);
		}
		else if (line.substr(0, 3) == "vt ") {
			std::istringstream str(line.substr(3));
			glm::vec2 vec;
			str >> vec.x; str >> vec.y;
			texCoords.push_back(vec);
		}
		else if (line.substr(0, 2) == "o ") {
			o_data mesh(line.substr(2));
			L_INFO("o " + mesh.name);
			meshes.push_back(mesh);
			currentMeshIdx++;
		}
		else if (line.substr(0, 2) == "f ") {
			if (currentMeshIdx < 0) {
				meshes.push_back(o_data());
				currentMeshIdx++;
			}
			std::string data = line.substr(2);
			std::vector<std::string> faces = split(data, ' ');
			for (int i = 0; i < faces.size(); ++i) {
				std::string face = faces[i];
				int v, vt, vn;
				try { getFaceIndices(face, &v, &vt, &vn); }
				catch (std::invalid_argument& e) 
				{ throw ObjFileCorruptedException(filepath); }
				meshes[currentMeshIdx].verticesIndices.push_back(v);
				meshes[currentMeshIdx].texCoordsIndices.push_back(vt);
				meshes[currentMeshIdx].normalsIndices.push_back(vn);
			}
		}
		else if (line.substr(0, 7) == "mtllib ") {
			std::string mtl_file = line.substr(7);
			L_INFO("mtllib " + mtl_file);
			try { parseMTL(mtl_file, materials); }
			catch(std::invalid_argument& e)
			{ throw MtlFileCorruptedException(directory + mtl_file); }
		}
		else if (line.substr(0, 7) == "usemtl ") {
			std::string material_name = split(line, ' ')[1];
			L_INFO("usemtl " + material_name);
			auto search = materials.find(material_name);
			if (search != materials.end()) {
				if (currentMeshIdx < 0) {
					meshes.push_back(o_data());
					currentMeshIdx++;
				}
				meshes[currentMeshIdx].o_material = new material(*(search->second));
			}
		}
	}
	for (auto iter = materials.begin(); iter != materials.end(); ++iter)
		delete iter->second;
	obj_file.close();
	L_INFO("Obj " + filepath + " parsed");
}

void ObjParser::parseMTL(std::string mtl_file_name, std::map<std::string, material*>& materials)
{
	std::string mtl_path = directory + mtl_file_name;
	std::ifstream mtl_file(mtl_path);
	if (!mtl_file.is_open()) {
		throw FileNotFoundException(mtl_path);
	}
	material* currentMaterial = NULL;
	std::string line;
	while (std::getline(mtl_file, line)) {
		std::vector<std::string> data = split(line, ' ');
		if (!data.empty()) {
			if (data[0] == "newmtl") {
				std::string name = line.substr(7);
				currentMaterial = new material(name);
				materials.insert(std::make_pair(name, currentMaterial));
			}
			else if (data[0] == "Ka") {
				if (currentMaterial) {
					glm::vec3 ka(0.0);
					ka.x = std::stof(data[1]);
					ka.y = std::stof(data[2]);
					ka.z = std::stof(data[3]);
					currentMaterial->Ka = ka;
				}
			}
			else if (data[0] == "Kd") {
				if (currentMaterial) {
					glm::vec3 kd(0.0);
					kd.x = std::stof(data[1]);
					kd.y = std::stof(data[2]);
					kd.z = std::stof(data[3]);
					currentMaterial->Kd = kd;
				}
			}
			else if (data[0] == "Ks") {
				if (currentMaterial) {
					glm::vec3 ks(0.0);
					ks.x = std::stof(data[1]);
					ks.y = std::stof(data[2]);
					ks.z = std::stof(data[3]);
					currentMaterial->Ks = ks;
				}
			}
			else if (data[0] == "Ns") {
				if (currentMaterial) {
					currentMaterial->Ns = std::stof(data[1]);
				}
			}
			else if (data[0] == "d") {
				if (currentMaterial) {
					currentMaterial->d = std::stof(data[1]);
				}
			}
			else if (data[0] == "illum") {
				if (currentMaterial) {
					currentMaterial->illum = std::stoi(data[1]);
				}
			}
			else if (data[0].substr(0, 3) == "map") {
				if (currentMaterial) {
					material::texture_map tm;
					tm.type = data[0]; tm.texture_fileName = data[1];
					currentMaterial->textures.push_back(tm);
				}
			}
		}
	}
}

void ObjParser::getFaceIndices(std::string face, int* v, int* vt, int* vn)
{
	*v = 0;  *vt = 0; *vn = 0;
	std::vector<std::string> face_components = split(face, '/');
	if (face_components.size() > 2)
		if (face_components[2].size() > 0)
			*vn = std::stoi(face_components[2]);
	if (face_components.size() > 1)
		if (face_components[1].size() > 0)
			*vt = std::stoi(face_components[1]);
	*v = std::stoi(face_components[0]);
}

std::string ObjParser::getDirectory(std::string filepath)
{
	int len = filepath.length();
	int i = len - 1;
	while (i >= 0 && filepath[i] != '/') --i;
	return filepath.substr(0, i + 1);
}

std::string ObjParser::Dir()
{
	return directory;
}
