#pragma once

#include <string>
#include "logger.h"
#include <SDL/SDL.h>
#include <GL/glew.h>

class GLWindow
{
public:
	unsigned int WIDTH;
	unsigned int HEIGHT;

	GLWindow(unsigned int width, unsigned int height, std::string title, bool* success);
	~GLWindow();
	void clear(float r, float g, float b, float a);
	void swap();
	SDL_Window* getWindow();
	void updateMousePosition();
	int MouseXOffset();
	int MouseYOffset();

private:
	GLWindow(const GLWindow& glwindow);

	SDL_Window* window;
	SDL_GLContext gl_context;

	int mouse_last_x, mouse_last_y;
	int mouse_x_offset, mouse_y_offset;

};