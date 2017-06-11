#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "Mesh.h"
#include <sstream>

#include <iterator>

struct material
{
	std::string name;
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	GLfloat Ns;
	GLfloat d;
	int illum;

	struct texture_map {
		std::string type;
		std::string texture_fileName;
	};

	std::vector<texture_map> textures;

	material(std::string name);

};

struct o_data
{
	std::string name;
	std::vector<GLuint> verticesIndices;
	std::vector<GLuint> normalsIndices;
	std::vector<GLuint> texCoordsIndices;

	material* o_material;

	o_data();
	~o_data();
	o_data(const o_data& o); //very important (meshes.push_back...)
	o_data(std::string o_name);
};

class ObjParser
{
public:
	ObjParser();
	void parse(std::string filepath);

	std::vector<glm::vec4> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;
	std::vector<o_data> meshes;

	std::string Dir();

private:
	std::string directory;
	int currentMeshIdx;
	static std::string getDirectory(std::string filepath);
	static void getFaceIndices(std::string face, int* v, int* vt, int* vn);
	void parseMTL(std::string mtl_file_name, std::map<std::string, material*>& materials);
};

class FileNotFoundException : public std::exception 
{
public:
	std::string m_msg;
	FileNotFoundException(std::string file) : m_msg("Unable to open file '" + file + "'") {}
	virtual const char* what() const throw() { return m_msg.c_str(); }
};
class ObjFileCorruptedException : public std::exception
{
public:
	std::string m_msg;
	ObjFileCorruptedException(std::string file) : m_msg("Obj file '" + file + "' corrupted") {}
	virtual const char* what() const throw() { return m_msg.c_str(); }
};

class MtlFileCorruptedException : public std::exception
{
public:
	std::string m_msg;
	MtlFileCorruptedException(std::string file) : m_msg("Mtl file '" + file + "' corrupted") {}
	virtual const char* what() const throw() { return m_msg.c_str(); }
};