//This class handles the player and the player's interaction with his environment.
//THIS CLASS IS CURRENTLY IN MAIN, SEE BELOW
//TODO: The functions of this class cannot be exported to it's own file for the following reasons:
//1) Dependency on functions currently declared in main
//2) SDL_Surface Screen and it's issues with usage between multiple files. (See window.cpp)
class Jack
{
private:
	//X and Y offsets of Jack
	double x, y;

	//Records the last y coordinate
	double yLast;

	//Velocity variables
	double xVel, yVel;

	//Jack's collision box
	SDL_Rect box;

	//Gravity probe
	SDL_Rect probe;

	//Current animation frame
	int frame;

	//Animation delay
	int delay;

	//Animation status
	int status;

	//Previous animation status
	int lastStatus;

	//Last known sprite direction
	int facing;

public:

	//Initalize class
	Jack();

	//Move Jack
	void move();

	//Jack's velocity
	void accel(int xAccel, int yAccel);

	//Stop horizontal movement
	void stop();

	//Show Jack
	void show();

	//Get current values from Jack
	/*	0 : x Position		1 : y Position
		2 : x Velocity		3 : y Velocity
		4 : onGround                        */
	double Read(int val);

	//Set Camera over Jack
	void set_camera();

	//Shift collision boxes
	void shift_boxes();

	//Clips Jacks Sprite Sheet
	void set_clips();

	//loads files for Jacks Sprites
	bool load_files();

	//Checks Collisions with platform and responds. STILL NEED TO FIX to work with a list of platforms, instead of just one.
	void Collide_Check(SDL_Rect plat);
};