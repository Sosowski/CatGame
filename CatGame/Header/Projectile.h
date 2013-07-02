#include "SDL.h"

//Current functionality can support different types of projectiles, move()'s base is set up to guide the bullet to a target,
//Could use this functionality to create a gun with hoaming bullets

//TODO : Projectile array in main
class Projectile
{
	//Handles projectiles for both player and enemies
private:
	double ProX; //Projectile coordinates
	double ProY;
	double speed; //Projectile speed
	int bullet_ID; //Tracks if player or foe shot bullet 1 - player 2 - enemy
	int type;      //The type of projectile. To be used later
	int damage;    //Damage of the bullet
	bool disabled; //If true, projectile is disabled.
	SDL_Rect target; //Where the bullet will travel


public:
	//Projectile constructor
	Projectile(double, double, double, SDL_Rect);

	//Check if the projectile is disabled
	bool is_disabled();

	//Calculates distance to target
	double distance(double x2, double x1, double y2, double y1);

	//Return x and y position
	double get_x();
	double get_y();

	void set_ProX(double);   //Set the projectile's position
	void set_ProY(double); 
	void set_speed(double); //Set projectile speed
	void set_bullet_ID(int); //Sets who the bullet was shot from
	void set_type(int);     //Bullet type
	void set_damage(int);    //Sets damage
	void collide();           //Collide with an enemy or player
	void move();             //Move projectile towards target
	void disable();          //Disable projectile and remove it from projectile array
};