#include "Model.h"
#include "ResourceManager.h"

Model::Model()
{

}

void Model::draw(ShaderProgram* shader)
{
	for (int i = 0; i < meshes.size(); ++i) {
		meshes[i]->draw(shader);
	}
}

Model* Model::fromFile(std::string path)
{
	Model* model = new Model();
	ObjParser parser;
	parser.parse(path);
	for (int i = 0; i < parser.meshes.size(); ++i) {
		int vertices_idx = -1;
		std::vector<Vertex> vertices; 
		std::vector<GLuint> indices;
		std::map<std::string, int> uniq_vertices;
		for (int j = 0; j < parser.meshes[i].verticesIndices.size(); ++j) {
			int v_idx = parser.meshes[i].verticesIndices[j];
			int vt_idx = parser.meshes[i].texCoordsIndices[j];
			int vn_idx = parser.meshes[i].normalsIndices[j];

			std::string key = std::to_string(v_idx) + std::to_string(vt_idx) + std::to_string(vn_idx);
			auto search = uniq_vertices.find(key);
			if (search == uniq_vertices.end()) {
				glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				glm::vec2 texCoord = glm::vec2(0.0f, 0.0f);
				glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);

				if (v_idx > 0) position = parser.vertices[v_idx - 1];
				if (vt_idx > 0) texCoord = parser.texCoords[vt_idx - 1];
				if (vn_idx > 0) normal = parser.normals[vn_idx - 1];

				++vertices_idx;
				vertices.push_back(Vertex(position, normal, texCoord));
				uniq_vertices.insert(std::make_pair(key, vertices_idx));
				indices.push_back(vertices_idx);
			}
			else {
				indices.push_back(search->second);
			}
		 }
		std::string mesh_name = parser.meshes[i].name;
		Material mesh_material;
		fetchMeshMaterial(mesh_material, parser, i);
		model->meshes.push_back(new Mesh(vertices, indices, mesh_material, mesh_name));
	}
	model->calculateBBox();
	return model;
}

void Model::fetchMeshMaterial(Material& material, ObjParser& parser, unsigned int mesh_idx)
{
	if (parser.meshes[mesh_idx].o_material) {
		material.ambient = parser.meshes[mesh_idx].o_material->Ka;
		material.diffuse = parser.meshes[mesh_idx].o_material->Kd;
		material.specular = parser.meshes[mesh_idx].o_material->Ks;
		material.shininess = parser.meshes[mesh_idx].o_material->Ns;

		L_INFO(parser.meshes[mesh_idx].o_material->name + " material [" + parser.meshes[mesh_idx].name + "]");
		std::cout << "ambient " << material.ambient.x << " " << material.ambient.y << " " << material.ambient.z << std::endl;
		std::cout << "diffuse " << material.diffuse.x << " " << material.diffuse.y << " " << material.diffuse.z << std::endl;
		std::cout << "specular " << material.specular.x << " " << material.specular.y << " " << material.specular.z << std::endl;
		std::cout << "shininess " << material.shininess << std::endl;


		for (int i = 0; i < parser.meshes[mesh_idx].o_material->textures.size(); ++i) {
			material::texture_map* tex = &parser.meshes[mesh_idx].o_material->textures[i];
			ResourceManager* loader = ResourceManager::getInstance();
			std::string texPath = parser.Dir() + tex->texture_fileName;
			std::cout << "Loading texture " << texPath << std::endl;
			GLuint tex_id = loader->getTexture(texPath);
			if (tex_id != -1) {
				std::string type = "";
				if (tex->type == "map_Ka") type = "texture_ambient";
				if (tex->type == "map_Kd") type = "texture_diffuse";
				if (tex->type == "map_Ks") type = "texture_specular";
				if (type.length() > 0) {
					material.textures.push_back(Texture(tex_id, type));
				}
			}
		}
	}
	
}

void Model::calculateBBox()
{
	bbox.min.x = bbox.max.x = meshes[0]->vertices[0].position.x;
	bbox.min.y = bbox.max.y = meshes[0]->vertices[0].position.y;
	bbox.min.z = bbox.max.z = meshes[0]->vertices[0].position.z;
	bbox.min.w = bbox.max.w = 1.0f;

	for (int i = 0; i < meshes.size(); ++i) {
		for (int j = 0; j < meshes[i]->vertices.size(); ++j) {
			if (meshes[i]->vertices[j].position.x < bbox.min.x) bbox.min.x = meshes[i]->vertices[j].position.x;
			if (meshes[i]->vertices[j].position.x > bbox.max.x) bbox.max.x = meshes[i]->vertices[j].position.x;
			if (meshes[i]->vertices[j].position.y < bbox.min.y) bbox.min.y = meshes[i]->vertices[j].position.y;
			if (meshes[i]->vertices[j].position.y > bbox.max.y) bbox.max.y = meshes[i]->vertices[j].position.y;
			if (meshes[i]->vertices[j].position.z < bbox.min.z) bbox.min.z = meshes[i]->vertices[j].position.z;
			if (meshes[i]->vertices[j].position.z > bbox.max.z) bbox.max.z = meshes[i]->vertices[j].position.z;
		}
	}
	bbox.center = glm::vec4((bbox.min.x + bbox.max.x) / 2.0, (bbox.min.y + bbox.max.y) / 2.0, (bbox.min.z + bbox.max.z) / 2.0, 1.0);
	bbox.center.w = 1.0f;
}

BBox Model::getBBox()
{
	return bbox;
}

Model::~Model()
{
	for (int i = 0; i < meshes.size(); ++i) {
		if (meshes[i]) {
			delete meshes[i];
		}
	}
	meshes.clear();
}

