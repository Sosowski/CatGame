class Timer
{
	//Creates timers nessisary for regulating FPS, and other time-related time functions. Time.
private:
	//Clock time when started
	int startTicks;

	//Clock time when paused
	int pausedTicks;

	//Returns current minute for clock formatting
	int min;

	//Timer status flags
	bool paused;
	bool started;

public:
	//Enables variables
	Timer();

	//Various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Get current time
	int get_ticks();

	//Get current time - Formatted for a clock display
	int get_ticks_clock();

	//Returns the current minute
	int return_min();

	//Check status
	bool is_started();
	bool is_paused();
};