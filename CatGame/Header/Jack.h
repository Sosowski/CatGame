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

	//Stops the collision detection if player has already hit a platform
	bool collided;

	//Flag sent to restore velocities back to 0 after a hit
	double knockbackX;

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

	//Get current Rectangles from Jack
	/*	0 : Hitbox		1 : Probe */
	SDL_Rect Read_rect(int val);

	SDL_Rect* get_camera();

	//Get specific camera values
	/*	0 : x Position		1 : y Position
		2 : Width   		3 : Height   */
	double get_camera_value(int val);

	double get_camera_value2(int val);

	//Set Camera over Jack
	void set_camera();

	//Set camera used for background objects
	void set_camera2();

	//Offset Jacks Camera
	void shift_camera(int xoffset, int yoffset);

	//Shift collision boxes
	void shift_boxes();

	//Clips Jacks Sprite Sheet
	void set_clips();

	//loads files for Jacks Sprites
	bool load_files();

	//Replacing with a response function, platforms now check for collision and call this function.
	void Collide_Response(bool hit, bool probe, int edge);

	//Handles Jack taking damage
	//Call with a number for power of hit
	void take_damage(int taken);

	//Returns player's current health
	int current_health();

	//Returns player's current health in a string for use in HUD
	std::string current_health_HUD();
};