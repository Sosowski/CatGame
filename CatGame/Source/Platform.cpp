#include "Library.h"

Platform::Platform()
{
	//Platform default hitboxes
	plat.x = 0;
	plat.y = 750;
	plat.w = 400;
	plat.h = 100;
	
	//Initialize variables for loading image
	platImg = NULL;
}

Platform::Platform(int xNew,int yNew, int wNew, int hNew, int type)
{
	plat.x = xNew;
	plat.y = yNew;
	plat.w = wNew;
	plat.h = hNew;

	platImg = NULL;
}

bool Platform::load_files(std::string img)
{
	this->platImg = load_image( img );
	if (this->platImg == NULL){
		return false;
	}
	else{
		return true;
	}
}

int Platform::Read(int val)
{
	int ret;
	switch (val)
	{
	case 0: // x value
		ret = plat.x;
		break;
	case 1: // y value
		ret = plat.y;
		break;
	case 2:	// width
		ret = plat.w;
		break;
	case 3:	// height
		ret = plat.h;
		break;
	default: // defualt is x value
		ret = plat.x;
		break;
	}
	return ret;
}

void Platform::show(Window& aWindow)
{
	aWindow.apply_surface( this->plat.x, this->plat.y, this->platImg, 1, &camera);
}

bool Platform::Collide(Jack& player)
{
	bool hit = false, touchFeet = false, touchHead=false, touchLeft=false, touchRight=false;
	int edge = 0;
	SDL_Rect thisPlat = this->plat;
	SDL_Rect hitbox = player.Read_rect(0);

	//check if the the player collides with overall bounds of platform
	if(check_collision(thisPlat,hitbox)){
		hit = true;
		//check if within height for a side collision
		if( hitbox.y+(hitbox.h*.75) > thisPlat.y ){
			//if player is on the left half of the platform, then his right touched, otherwise it is his left.
			if( hitbox.x+(hitbox.w/2) < thisPlat.x+(thisPlat.w/2) ){
				touchRight = true;
				edge = thisPlat.x;
			}
			else{
				touchLeft = true;
				edge = thisPlat.x + thisPlat.w;
			}
			//check for head collision
			if( hitbox.y-1 < thisPlat.y+thisPlat.h && hitbox.y > thisPlat.y ){
				if(hitbox.x+(hitbox.w/2) > thisPlat.x || hitbox.x+(hitbox.w/2) < thisPlat.x+thisPlat.w){
					touchHead = true;
					edge = thisPlat.y+thisPlat.h;
				}
			}
		}
		//check if feet are colliding
		else if(check_collision(thisPlat,player.Read_rect(1))){ // may not need foot probe in Jack later
			touchFeet = true;
			edge = thisPlat.y;
		}
		player.Collide_Response(touchFeet,touchHead, touchRight, touchLeft, edge, this->plat);
		return true;
	}
	else{
		hit = false;
		return false;
	}
	/*
//New plan: Move() calls a function to the platform object it has stored in the touchSide pointer.
//this function checks the conditions in which to unset flags.
//Scratch that, but clearing the flags still happens in move()
//Head colision is simple at the moment, does not set a flag, but instead resets the player position.
the velocity then propels upward, and a collision is detected again, so its a lot of collisions. We may want to 
reconsider. how this is done. Not sure how efficient this method is, but it could be used for side collisions if
it ends up just as effecient or more effiecient than the current method.*/
}

void Platform::clean_up(){
	SDL_FreeSurface ( platImg );
}

Triangle::Triangle()
{
	//Platform default hitboxes
	plat.x = 0;
	plat.y = 750;
	plat.w = 400;
	plat.h = 100;

	//Initialize variables for loading image
	platImg = NULL;
}

bool Triangle::Collide(Jack& player)
{
	bool hit = false, touchFeet = false, touchHead=false, touchLeft=false, touchRight=false;
	int edge = 0;
	SDL_Rect thisPlat = this->plat;
	SDL_Rect hitbox = player.Read_rect(0);

	//check if the the player collides with overall bounds of platform
	if(check_collision(thisPlat,hitbox)){
		hit = true;
		//check if within height for a side collision
		if( hitbox.y+(hitbox.h*.75) > thisPlat.y ){
			//if player is on the left half of the platform, then his right touched, otherwise it is his left.
			if( hitbox.x+(hitbox.w/2) < thisPlat.x+(thisPlat.x/2) ){
				touchRight = true;
				edge = thisPlat.x;
			}
			else{
				touchLeft = true;
				edge = thisPlat.x + thisPlat.w;
			}
			//check for head collision
			if( hitbox.y-1 < thisPlat.y+thisPlat.h ){
				touchHead = true;
				//edge = thisPlat.y+thisPlat.h;
			}
		}
		//check if feet are colliding
		else if(check_collision(thisPlat,player.Read_rect(1))){ // may not need foot probe in Jack later
			touchFeet = true;
			edge = thisPlat.y;
		}
		player.Collide_Response(touchFeet,touchHead, touchRight, touchLeft, edge, this->plat);
		return true;
	}
	else{
		hit = false;
		return false;
	}
}