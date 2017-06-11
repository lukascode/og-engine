#pragma once

#include <SDL/SDL.h>

class Timer
{
public:
	Timer();

	void start(Uint32 initTime = SDL_GetTicks());
	void stop();
	void pause();
	void unpause();

	Uint32 getTicks();

	//timer status
	bool isStarted();
	bool isPaused();

private:
	Uint32 startTicks;
	Uint32 pausedTicks;

	bool paused;
	bool started;
};