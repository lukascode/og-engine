#include "ResourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

ResourceManager* ResourceManager::instance = 0;

ResourceManager* ResourceManager::getInstance()
{
	if (!instance) instance = new ResourceManager();
	return instance;
}

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
	for (auto iter = models.begin(); iter != models.end(); ++iter)
		if (iter->second) delete iter->second;
	for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
		if (iter->second) delete iter->second;
	
}

Model* ResourceManager::getModel(std::string filepath)
{
	Model* model;
	auto search = models.find(filepath);
	if (search == models.end()) {
		try {
			model = Model::fromFile(filepath);
		}
		catch (const std::exception& e) {
			L_ERROR(std::string(e.what()));
			model = 0;
		}
		if (model) models.insert(std::make_pair(filepath, model));
	}
	else {
		model = search->second;
	}
	return model;
}

ShaderProgram* ResourceManager::getShader(std::string vertex, std::string fragment)
{
	ShaderProgram* shader;
	auto search = shaders.find(vertex + fragment);
	if (search == shaders.end()) {
		try {
			shader = new ShaderProgram(vertex, fragment);
		}
		catch (std::exception& e) {
			L_ERROR(std::string(e.what()) + " " + vertex + " " + fragment);
			shader = 0;
		}
		if (shader) shaders.insert(std::make_pair(vertex + fragment, shader));
	}
	else {
		shader = search->second;
	}
	return shader;
}

GLint ResourceManager::getTexture(std::string filepath)
{
	auto search = textures.find(filepath);
	if (search == textures.end()) {
		int width, height, nrComponents;
		unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrComponents, 0);
		std::cout << filepath << " width: " << width << " height: " << height << std::endl;
		if (data) {
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			GLuint tex_id;
			glGenTextures(1, &tex_id);
			glBindTexture(GL_TEXTURE_2D, tex_id);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			stbi_image_free(data);

			textures.insert(std::make_pair(filepath, tex_id));
			return tex_id;
		}
		else {
			L_ERROR("Failed to load texture " + filepath);
			return -1;
		}

	}
	else {
		return search->second;
	}
}

