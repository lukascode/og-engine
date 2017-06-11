#pragma once 

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "logger.h"
#include <exception>
#include <vector>


class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(std::string vs, std::string fs);
	~ShaderProgram();

	GLuint get();
	void use();
	void addShader(std::string filename, GLenum type);
	void build();
	GLint getAttribLocation(std::string name);
	GLint getUniformLocation(std::string name);
	void set1f(std::string name, GLfloat value);
	void set1i(std::string name, GLint value);
	void set3f(std::string name, glm::vec3 value);
private:

	GLuint program;
	std::vector<GLuint> shaders;
	void remove_shaders();

	static GLuint create_shader(const char* filename, GLenum type);
	static char* file_read(const char* filename);
    static void print_log(GLuint object);
};

class ShaderCreationException : public std::exception 
{
public:
	virtual const char* what() const throw() { return "ShaderCreationException"; }
};
class LinkProgramException : public std::exception 
{
public:
	virtual const char* what() const throw() { return "LinkProgramException"; }
};