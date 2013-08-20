class Platform
{
protected:
	//Platform collision boxes
	SDL_Rect plat;
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

	//Checks for collions, calls the function In Jack to react to collision.
	virtual bool Collide(Jack& player);
};

//should be a subclass of platform, allows polymorphism
class Triangle: public Platform
{
public:
	Triangle();
	bool Collide(Jack& player);
};