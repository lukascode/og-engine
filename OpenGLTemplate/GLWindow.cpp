#include "GLWindow.h"

static const bool FULL_SCREEN = false;
static const bool CURSOR_VISIBLE = true;

GLWindow::GLWindow(unsigned int width, unsigned int height, std::string title, bool* success) : WIDTH(width), HEIGHT(height)
{
	bool result = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		L_ERROR("unable to initialize SDL: " + std::string(SDL_GetError()));
		result = false;
	}
	/* configuration */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	/* window & context */
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	if (window == NULL) {
		L_ERROR("can't create window: " + std::string(SDL_GetError()));
		result = false;
	}

	
	if(!CURSOR_VISIBLE)	SDL_ShowCursor(SDL_DISABLE);
	//SDL_SetRelativeMouseMode(SDL_TRUE); //trap cursor in window (SDL_WarpMouseInWindow not working)
	
	if(FULL_SCREEN)
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	mouse_last_x = 0;
	mouse_last_y = 0;
	mouse_x_offset = 0;
	mouse_y_offset = 0;

	gl_context = SDL_GL_CreateContext(window);
	if (!gl_context) {
		L_ERROR("SDL_GL_CreateContext: " + std::string(SDL_GetError()));
		result = false;
	}

	/* Extension wrangler initialising */
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		void* err = (void*)glewGetErrorString(glew_status);
		L_ERROR("glewInit: " + std::string((char*)err));
		result = false;
	}
	*success = result;
}

GLWindow::~GLWindow()
{
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void GLWindow::clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWindow::swap()
{
	SDL_GL_SwapWindow(window);
}

SDL_Window* GLWindow::getWindow()
{
	return window;
}

//when cursor get to right edge, it 'll back to left edge etc
void GLWindow::updateMousePosition()
{
	int xpos, ypos; SDL_GetMouseState(&xpos, &ypos);
	static bool firstMouse = true;
	if (firstMouse) {
		mouse_last_x = xpos;
		mouse_last_y = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - mouse_last_x;
	GLfloat yoffset = ypos - mouse_last_y;
	mouse_last_x = xpos;
	mouse_last_y = ypos;

	if (xoffset < 0 && xpos == 0) {
		SDL_WarpMouseInWindow(window, WIDTH, ypos);
		mouse_last_x = WIDTH;
	}
	else if (xoffset > 0 && xpos <= WIDTH && xpos >= WIDTH - 1) {
		SDL_WarpMouseInWindow(window, 0, ypos);
		mouse_last_x = 0;
	}
	mouse_x_offset = xoffset;
	mouse_y_offset = yoffset;
}

int GLWindow::MouseXOffset()
{
	return mouse_x_offset;
}

int GLWindow::MouseYOffset()
{
	return mouse_y_offset;
}