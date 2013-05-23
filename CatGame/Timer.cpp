#include "Library.h"

Timer::Timer()
{
	//Enable variables, set flags
	startTicks = 0;
	pausedTicks = 0;
	paused = false;
	started = false;
}

//Start Timer

void Timer::start()
{
	//Set running flag
	started = true;

	//Set paused flag
	paused = false;

	//Get current time
	startTicks = SDL_GetTicks();
}

//Stop Timer

void Timer::stop()
{
	//Stop running flag
	started = false;

	//Disable paused flag
	paused = false;
}

//Get Current Time

int Timer::get_ticks()
{
	//If timer running
	if( started == true )
	{
		//If paused
		if( paused == true )
		{
			//return time that timer was paused at
			return pausedTicks;
		}
		else
		{
			//Return current time
			return SDL_GetTicks() - startTicks;
		}
	}
	//If timer isn't even running
	return 0;
}

//Pause Timer
void Timer::pause()
{
	//Check if timer is running and unpaused
	if((started == true) && ( paused == false))
	{
		//Set pause flag
		paused = true;

		//Save the time before the pause
		pausedTicks = SDL_GetTicks() - startTicks;
	}
}

//Unpause Timer
void Timer::unpause()
{
	//If timer is paused
	if( paused == true )
	{
		//Unpause
		paused = false;

		//Reset start ticks
		startTicks = SDL_GetTicks() - pausedTicks;

		//Reset paused ticks
		pausedTicks = 0;
	}
}

//Send current flags upon request
bool Timer::is_started()
{
	return started;
}

bool Timer::is_paused()
{
	return paused;
}