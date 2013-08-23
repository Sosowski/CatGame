class Platform
{
protected:
	//Platform collision boxes
	SDL_Rect plat;

	//img for the platform
	SDL_Surface *platImg;

public:
	//Initalize class
	Platform();
	Platform(int xNew, int yNew, int wNew, int hNew, int type);

	bool load_files(std::string img);
	
	void show(Window& aWindow);

	//Get current values from this platform
	/*	0 : x Position		1 : y Position
		2 : Width			3 : Height
	*/
	int Read(int val);

	//Checks for collions, calls the function In Jack to react to collision.
	virtual bool Collide(Jack& player);

	void Platform::clean_up();
};

//should be a subclass of platform, allows polymorphism
class Triangle: public Platform
{
public:
	Triangle();
	bool Collide(Jack& player);
};