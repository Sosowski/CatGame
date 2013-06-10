class Platform
{
private:
	//Platform collision boxes
	SDL_Rect plat;
	
	//Selector for different platform images
	int platType;

public:
	//Initalize class
	Platform();
	Platform(int xNew, int yNew, int wNew, int hNew, int type);

	bool load_files();
	
	void show(Window& aWindow);

	//Get current values from this platform
	/*	0 : x Position		1 : y Position
		2 : Width			3 : Height
		4 : Type */
	int Read(int val);

	//Gives loaded images files to other documents
	SDL_Surface *Return(int val);
};
