class Jack
{
private:
	//X and Y offsets of Jack
	int x, y;

	//Offets for Jacks camera
	int xOffset, yOffset, xOff, yOff, xDiff, yDiff;
	//temporary Easing values
	int EaseIn, EaseOut, xCamVel, yCamVel;

	//Records the last y coordinate
	int yLast;

	//Velocity variables
	double xVel, yVel;

	//flags for if left and right key are being pressed.
	bool left, right;

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

	//Walk function. The following are when happen on key up and key down.
	//	0 : left down		1 : right down
	//	2 : left up			3 : right up
	void walk(int dir);

	//Stop horizontal movement
	void stop();

	//Show Jack
	void show(Window& aWindow);

	//Get current values from Jack
	/*	0 : x Position		1 : y Position
		2 : x Velocity		3 : y Velocity
		4 : onGround                        */
	double Read(int val);

	SDL_Rect* get_camera();

	//Get specific camera values
	/*	0 : x Position		1 : y Position
		2 : Width   		3 : Height   */
	double get_camera_value(int val);

	//Set Camera over Jack
	void set_camera();

	//Offset Jacks Camera
	void shift_camera(int xoffset, int yoffset);

	//Shift collision boxes
	void shift_boxes();

	//Clips Jacks Sprite Sheet
	void set_clips();

	//loads files for Jacks Sprites
	bool load_files();

	//Checks Collisions with platform and responds. STILL NEED TO FIX to work with a list of platforms, instead of just one.
	//Added an int where if it is 1, it signifies that it is the last platform to be checked
	void Collide_Check(SDL_Rect plat, int check);
};