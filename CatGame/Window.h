class Window
{
	//This class handles all nessisary setup and event handling for the program.
	//Mouse and keyboard focus as well as fullscreen are here.
	//TODO: Edit class to allow for an options menu?
private:
	//Flag if currently windowed
	bool windowed;

	//Flag if window is set up fine
	bool windowOK;

	SDL_Surface *screen;

public:
	//constructor
	Window();

	//Handle window events
	void handle_events();

	//Toggle fullscreen
	void toggle_fullscreen();

	//Return any errors
	bool error();

	// Returns a pointer to the screen.
	SDL_Surface* getScreen();
};