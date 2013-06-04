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
	SDL_Surface* getScreen();
	// Returns a pointer to the background.
	SDL_Surface* getBG();

	void showBG();

	void update_Cam();

	void set_target(SDL_Rect* tar);

	void set_target(SDL_Rect* tar, int Vel, int ease_in, int ease_out);

	//Apply an image onto screen
	void apply_surface ( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );
	void apply_surface ( int x, int y, int sou, int dest, SDL_Rect* clip);
	void apply_surface ( int x, int y, SDL_Surface* source, int dest, SDL_Rect* clip);
	void apply_surface ( int x, int y, int sou, SDL_Surface* destination, SDL_Rect* clip);
};