#include "Timer.h"

Timer::Timer()
{
	startTicks = 0;
	pausedTicks = 0;
	
	paused = false;
	started = false;
}

void Timer::start(Uint32 initTime)
{
	startTicks = initTime;
	pausedTicks = 0;
	paused = false;
	started = true;
}

void Timer::stop()
{
	started = false;
	paused = false;
	startTicks = pausedTicks = 0;
}

void Timer::pause()
{
	if (started && !paused) {
		paused = true;
		pausedTicks = SDL_GetTicks() - startTicks;
		startTicks = 0;
	}
}

void Timer::unpause()
{
	if (started && paused) {
		paused = false;
		startTicks = SDL_GetTicks() - pausedTicks;
		pausedTicks = 0;
	}
}

Uint32 Timer::getTicks()
{
	Uint32 time = 0;
	if (started) {
		if (paused) return pausedTicks;
		else time = SDL_GetTicks() - startTicks;
	}
	return time;
}

bool Timer::isStarted()
{
	return started;
}

bool Timer::isPaused()
{
	return paused && started;
}

