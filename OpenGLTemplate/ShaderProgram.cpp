#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
	program = glCreateProgram();
}

ShaderProgram::ShaderProgram(std::string vs, std::string fs)
{
	L_INFO("Shaders compilation " + vs + " " + fs);
	program = glCreateProgram();
	this->addShader(vs, GL_VERTEX_SHADER);
	this->addShader(fs, GL_FRAGMENT_SHADER);
	this->build();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(program);
}

GLuint ShaderProgram::get()
{
	return this->program;
}

void ShaderProgram::use()
{
	glUseProgram(program);
}

GLint ShaderProgram::getAttribLocation(std::string name)
{
	return glGetAttribLocation(this->program, name.c_str());
}

GLint ShaderProgram::getUniformLocation(std::string name)
{
	return glGetUniformLocation(this->program, name.c_str());
}

void ShaderProgram::set1f(std::string name, GLfloat value)
{
	glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::set1i(std::string name, GLint value)
{
	glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::set3f(std::string name, glm::vec3 value)
{
	glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
}

void ShaderProgram::addShader(std::string filename, GLenum type)
{
	GLuint shader_ref = create_shader(filename.c_str(), type);
	if (shader_ref == 0)
		throw ShaderCreationException();
	shaders.push_back(shader_ref);
}

void ShaderProgram::build()
{
	for (int i = 0; i < shaders.size(); ++i) {
		glAttachShader(program, shaders[i]);
	}
	glLinkProgram(program);
	GLint link_ok = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		L_ERROR("glLinkProgram error");
		print_log(program);
		throw LinkProgramException();
	}
	remove_shaders();
}

void ShaderProgram::remove_shaders()
{
	while (!shaders.empty()) {
		GLuint shader = shaders.back();
		shaders.pop_back();
		glDetachShader(program, shader);
		glDeleteShader(shader);
	}
}

GLuint ShaderProgram::create_shader(const char* filename, GLenum type)
{
	const GLchar* source = file_read(filename);
	if (source == NULL) {
		L_ERROR(std::string(SDL_GetError()));
		return 0;
	}
	GLuint res = glCreateShader(type);
	const GLchar* sources[] = {
		source
	};
	glShaderSource(res, 1, sources, NULL);
	free((void*)source);
	glCompileShader(res);
	GLint compile_ok = GL_FALSE;
	glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
	if (compile_ok == GL_FALSE) {
		L_ERROR("could not compile file: " + std::string(filename));
		print_log(res);
		glDeleteShader(res);
		return 0;
	}
	return res;
}

void ShaderProgram::print_log(GLuint object)
{
	GLint log_length = 0;
	if (glIsShader(object)) {
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	} 
	else if (glIsProgram(object)) {
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	}
	else {
		L_ERROR("print_log: Not a shader or program");
		return;
	}
	char* log = (char*)malloc(log_length);
	if (glIsShader(object))
		glGetShaderInfoLog(object, log_length, &log_length, log);
	else if (glIsProgram(object))
		glGetProgramInfoLog(object, log_length, &log_length, log);
	L_ERROR(std::string(log));
	free(log);
}

char* ShaderProgram::file_read(const char* filename)
{
	SDL_RWops* rw = SDL_RWFromFile(filename, "rb");
	if (rw == NULL) return NULL;

	Sint64 res_size = SDL_RWsize(rw);
	char* res = (char*)malloc(res_size + 1);

	Sint64 nb_read_total = 0, nb_read = 1;
	char* buf = res;
	while (nb_read_total < res_size && nb_read != 0) {
		nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
		nb_read_total += nb_read;
		buf += nb_read;
	}
	SDL_RWclose(rw);
	if (nb_read_total != res_size) {
		free(res);
		return NULL;
	}
	res[nb_read_total] = '\0';
	return res;
}