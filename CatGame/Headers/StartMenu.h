//This class controls player interactivity with the start menu.

class StartMenu
{
private:

	//The current state of the button
	int status;

public:
	//Initalize class
	StartMenu();

	//Load associated files
	bool load_files();

	//Clip image files
	void clip_images();

	//Show button based on state
	void show(Window& aWindow, int status);

	//Return the GAME_STATE flag
	int state_return();

	//Returns the current hitbox for buttons
	SDL_Rect button_box();
};