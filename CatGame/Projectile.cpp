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