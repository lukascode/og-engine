#pragma once
#include <map>
#include "Model.h"
#include "ShaderProgram.h"

class ResourceManager
{
public:
	static ResourceManager* getInstance();

	Model* getModel(std::string filepath);
	ShaderProgram* getShader(std::string vertex, std::string fragment);
	GLint getTexture(std::string filepath);

private:
	ResourceManager();
	~ResourceManager();
	static ResourceManager* instance;

	std::map<std::string, Model*> models;
	std::map<std::string, ShaderProgram*> shaders;
	std::map<std::string, GLuint> textures;
};
