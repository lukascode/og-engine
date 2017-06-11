#include <iostream>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include "logger.h"
#include "ShaderProgram.h"
#include <stdio.h>
#include "GLWindow.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Timer.h"
#include "Camera.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"

#include "Cube.h"

#include <vector>
#include <string>

#include "Model.h"

#include <SOIL/SOIL.h>

#include "Object.h"
#include "Scene.h"
#include "ResourceManager.h"


class OpenGLModels
{

private:
	ShaderProgram* model_shader;
	GLWindow* gl_window;
	Camera* camera;
	Scene* scene;
	bool config_window;

	glm::mat4 projection;

public:
	OpenGLModels()
	{
		model_shader = 0;
		gl_window = 0;
		camera = 0;
		scene = 0;
		config_window = false;

	}

	~OpenGLModels()
	{
		if (model_shader) delete model_shader;
		if (gl_window) delete gl_window;
		if (camera) delete camera;
		if (scene) delete scene;
	}


	int run()
	{
		bool success;
		gl_window = new GLWindow(1200, 640, "OpenGL Models", &success);
		if (!success) return EXIT_FAILURE;

		// Setup ImGui binding
		ImGui_ImplSdlGL3_Init(gl_window->getWindow());

		//viewport
		glViewport(0, 0, gl_window->WIDTH, gl_window->HEIGHT);

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);

		//blending (transparency)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		//shaders
		model_shader = ResourceManager::getInstance()->getShader("res/mesh.v.glsl", "res/mesh.f.glsl");

		//game objects
		camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

		//models
		Object* object;
		object = new Object("res/nanosuit/nanosuit.obj", &projection, model_shader); object->position = glm::vec3(0.0, 1.0, 0.0);
		///object = new Object("res/needle01/needle01.obj", projection, model_shader);
		///object = new Object("res/m4a1.obj", projection, model_shader);
		///object = new Object("res/dog_triangles.obj", projection, model_shader); object->scale_factor = glm::vec3(0.1);
		Object* object2 = new Object("res/chibi_triangles.obj", &projection, model_shader); object2->scale_factor = glm::vec3(0.1);
		object2->position = glm::vec3(0.0, 1.0, 20.0);

		Object* object3 = new Object("res/needle01/needle01.obj", &projection, model_shader);
		object3->position = glm::vec3(-20.0, 1.0, 10.0);
		object3->scale_factor = glm::vec3(3.0);



		scene = new Scene();
		scene->root->object = object;
		scene->root->addChild(object2);
		scene->root->addChild(object3);
		//generateForest();

		


		//keys
		bool keys[1024] = { 0 };

		//fps and timer
		const int FPS = 60;
		Timer fpsTimer;
		GLfloat deltaTime = 0.0f;
		GLfloat lastFrame = 0.0f;


		SDL_Event ev;
		while (true) {
			fpsTimer.start();
			GLfloat currentFrame = SDL_GetTicks();
			deltaTime = (currentFrame - lastFrame) / 1000.0f;
			lastFrame = currentFrame;

			//imgui event processing
			ImGui_ImplSdlGL3_ProcessEvent(&ev);

			while (SDL_PollEvent(&ev)) {
				if (ev.type == SDL_QUIT)
					return 0;
				if (ev.type == SDL_KEYUP) {
					keys[ev.key.keysym.sym] = false;
					if (ev.key.keysym.sym == SDLK_c)
						config_window = !config_window;
				}
				else if (ev.type == SDL_KEYDOWN) {
					keys[ev.key.keysym.sym] = true;
					if (ev.key.keysym.sym == SDLK_ESCAPE)
						return 0;
				}
				else if (ev.type == SDL_MOUSEMOTION) {
					gl_window->updateMousePosition();
					if (!config_window) {
						camera->MotionHandler(gl_window->MouseXOffset(), -gl_window->MouseYOffset());
					}
				}
				//zooming
				else if (ev.type == SDL_MOUSEWHEEL) {
					GLfloat yoffset = ev.wheel.y;
					if (yoffset > 0)
						camera->zoomIn();
					else camera->zoomOut();
				}
			}
			//new frame imgui
			ImGui_ImplSdlGL3_NewFrame(gl_window->getWindow());

			//check input
			if (!config_window) {
				if (keys[SDLK_w]) camera->move(FORWARD, deltaTime);
				if (keys[SDLK_s]) camera->move(BACKWARD, deltaTime);
				if (keys[SDLK_a]) camera->move(LEFT, deltaTime);
				if (keys[SDLK_d]) camera->move(RIGHT, deltaTime);
				if (keys[SDLK_r]) camera->setPosition(glm::vec3(0.0, 0.0, 0.0));
			}
			if (keys[SDLK_SPACE]) camera->zoomReset();

			// Clear the screen
			gl_window->clear(1.0, 1.0, 1.0, 1.0);
			//gl_window->clear(0.1, 0.1, 0.1, 1.0);


			//update model
		//	object->rotation_angle_y = SDL_GetTicks() / 60.0f;

			//projection matrix
			projection = glm::perspective(glm::radians(camera->getZoom()), (GLfloat)gl_window->WIDTH / (GLfloat)gl_window->HEIGHT, 0.1f, 400.0f);

			renderScene();
			renderGUI();

			//imgui render
			ImGui::Render();
			gl_window->swap();

			//if frame finish early,wait
			int frameTicks = fpsTimer.getTicks();
			int ticksPerFrame = 1000 / FPS;
			if (frameTicks < ticksPerFrame) {
				SDL_Delay(ticksPerFrame - frameTicks);
			}
		}
		
		// Cleanup
		ImGui_ImplSdlGL3_Shutdown();
		return 0;
	}

	void renderScene()
	{
		scene->draw(camera);
		
	}

	void renderGUI()
	{
		if (config_window) {
			ImGuiWindowFlags window_flags =
				ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
			ImGui::SetNextWindowSize(ImVec2(380, 360), ImGuiSetCond_FirstUseEver);
			ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Configuration", &config_window, window_flags);

			ImGui::Text("Configuration");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Separator();

			ImGui::End();
		}
	}

	void generateForest()
	{
		const int TREES_NUMBER = 80;
		const int RANGE = 150.0f;
		Model* tree_model = ResourceManager::getInstance()->getModel("res/needle01/needle01.obj");
		//scene->root->object = new Object(tree_model, &projection, model_shader);	
		for (int i = 0; i < TREES_NUMBER; ++i) {
			glm::vec3 pos(0.0, 0.0, 0.0);
			pos.x = RandomFloat(-RANGE, RANGE);
			pos.z = RandomFloat(-RANGE, RANGE);
			Object* object = new Object(tree_model, &projection, model_shader, pos);
			object->scale_factor = glm::vec3(3.0f);
			scene->root->addChild(object);
		}
	}

	float RandomFloat(float a, float b) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}
};

int main(int, char**)
{
	srand(time(0));
	OpenGLModels test; return test.run();

	return 0;
}

