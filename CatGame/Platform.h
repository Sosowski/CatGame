class Platform
{
private:
	//Platform collision boxes
	SDL_Rect plat;
public:
	//Initalize class
	Platform();
	Platform(int xNew,int yNew, int wNew, int hNew);

	bool load_files();
	
	void show(Window& aWindow);

	//Get current values from this platform
	/*	0 : x Position		1 : y Position
		2 : Width			3 : Height	*/
	int Read(int val);
};
