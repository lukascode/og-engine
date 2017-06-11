#pragma once
#include "Mesh.h"
#include "ObjParser.h"
#include <map>
#include <string>

struct BBox
{
	glm::vec4 min;
	glm::vec4 max;
	glm::vec4 center;
};

class Model
{
public:
	~Model();
	static Model* fromFile(std::string path);
	void draw(ShaderProgram* shader);
	BBox getBBox();
private:
	Model();
	std::vector<Mesh*> meshes;
	BBox bbox;
	static void fetchMeshMaterial(Material& material, ObjParser& parser, unsigned int mesh_idx);
	void calculateBBox();
};