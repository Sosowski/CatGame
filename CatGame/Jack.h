#include "SDL.h"

//This class handles the player and the player's interaction with his environment.
//THIS CLASS IS CURRENTLY IN MAIN, SEE BELOW
//TODO: The functions of this class cannot be exported to it's own file for the following reasons:
//1) Dependency on functions currently declared in main
//2) SDL_Surface Screen and it's issues with usage between multiple files. (See window.cpp)
class Jack
{
private:
	//X and Y offsets of Jack
	float x, y;

	//Records the last y coordinate
	float yLast;

	//Velocity variables
	float xVel, yVel;

	//Jack's collision box
	SDL_Rect box;

	//Gravity probe
	SDL_Rect probe;

	//Platform collision boxes
	SDL_Rect plat1;
	SDL_Rect plat2;
	SDL_Rect plat3;

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

	// ---- functions for controling Jack ----
	//Adds Velocity to jack
	void accel(int xAccel, int yAccel);

	void stop(); // Stops horizontal movement

	//Show Jack
	void show();

	// ---- Function for Reading from Jack ----
	/*	0 : x Position		1 : y Position
		2 : x Velocity		3 : y Velocity	*/
	float Read(int val);

	//Set Camera over Jack
	void set_camera();

	//Shift collision boxes
	void shift_boxes();

	//Clips Jacks Sprite Sheet
	void set_clips();

	//loads files for Jacks Sprites
	bool load_files();
};