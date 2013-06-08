#include "Library.h"

Projectile::Projectile(double pProX, double pProY, double pspeed, SDL_Rect ptarget)
{
	ProX = pProX;
	ProY = pProY;
	target = ptarget;
	speed = pspeed;
	bullet_ID = 0;
	type = 0;
	damage = 0;
	disabled = false;
}

double Projectile::get_x()
{
        //Returns the current x position
        return ProX;
}

double Projectile::get_y()
{
        //Returns the current y position
        return ProY;
}

bool Projectile::is_disabled()
{
	return disabled;
}

void Projectile::set_ProX(double a){
	ProX = a;
}

void Projectile::set_ProY(double a){
	ProY = a;
}

void Projectile::set_speed(double a){
	speed = a;
}

void Projectile::set_bullet_ID(int a){
	bullet_ID = a;
}

void Projectile::set_type(int a){
	type = a;
}

void Projectile::set_damage(int a){
	damage = a;
}

void Projectile::collide(){

	//Health reduction will be done here, for now, everything dies in one shot
	disable();

}

void Projectile::disable()
{
        disabled = true;
}

double Projectile::distance(double x2, double x1, double y2, double y1)
{
        return sqrt(pow(x2-x1, 2)+pow(y2-y1, 2));
}


void Projectile::move()
{
	//Target position
	int tar_x = 0, tar_y = 0;

	//Used to create unit vector to make bullet travel
	double unit_x = 0, unit_y = 0;

	double dist = 0.0;

	//Set target position to the SDL_Rect target
	tar_x = target.x;
	tar_y = target.y;

	//Make a unit vector to make the projectile travel
	dist = distance(tar_x, ProX, tar_y, ProY);
	unit_x = (tar_x - ProX) / dist;
	unit_y = (tar_y - ProY) / dist;

	if(dist <= speed)
	{ //Projectile is going to hit target
	  //TODO: edit to allow projectile to continue traveling past target
		ProX = tar_x;
		ProY = tar_y;
		//Disable the projectile
		collide();
	}
	else
	{ //No collision yet ; keep traveling
		unit_x *= speed;   //Projectile moves as fast as speed is set
		unit_y *= speed;
		ProX += unit_x;
		ProY += unit_y;
	}

}