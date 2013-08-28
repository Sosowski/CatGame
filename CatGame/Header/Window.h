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

	SDL_Window *screen;
	SDL_Renderer *sdlRenderer;
	//SDL_Surface *screen;
	SDL_Rect *target;		// for camera
	//SDL_Surface *background;

	int EaseIn;
	int EaseOut;
	int xDiff;
	int yDiff;
	int xVel;
	int yVel;

public:
	//constructor
	Window();

	//Handle window events
	void handle_events();

	//Toggle fullscreen
	void toggle_fullscreen();

	//Return any errors
	bool error();

	bool load_files(std::string& BgImage);
	
	// Returns a pointer to the screen.
	SDL_Window* getScreen();
	//Returns a pointer to the renderer
	SDL_Renderer* getRenderer();
	// Returns a pointer to the background.
	SDL_Surface* getBG();

	void showBG();

	void update_Cam();

	void set_target(SDL_Rect* tar);

	void set_target(SDL_Rect* tar, int Vel, int ease_in, int ease_out);

	/* This Applys a viewable image to the scene.
	This uses overloading to provide diferent ways to apply something to the screen.
	It may be worth while to check if all are necesary, I did not completely test each case.
	When using the cases with integers:
		0: screen	1: background		
	The first function is the origional function, Unchanged except that it is now a member of this class	*/
	void apply_surface ( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );
	//The following is used to apply the background to the screen. Possibly other layers in the future.
	void apply_surface ( int x, int y, int sou, int dest, SDL_Rect* clip);
	//The following function is best for applying to the screen in relation to the current camera. Pass a 1 for destination.
	void apply_surface ( int x, int y, SDL_Surface* source, int dest, SDL_Rect* clip);
	//void apply_surface ( int x, int y, int sou, SDL_Surface* destination, SDL_Rect* clip);

	//sends the current state of windowOK
	bool return_windowOK();
};