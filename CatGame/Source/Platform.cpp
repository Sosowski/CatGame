#include "Library.h"

Platform::Platform()
{
	//Platform default hitboxes
	plat.x = 0;
	plat.y = 750;
	plat.w = 400;
	plat.h = 100;
	//Initialize variables for loading image
	//plat1 = NULL;
	platType = 1;

	load_files();
}

Platform::Platform(int xNew,int yNew, int wNew, int hNew, int type)
{
	plat.x = xNew;
	plat.y = yNew;
	plat.w = wNew;
	plat.h = hNew;
	platType = type;

	load_files();
}

bool Platform::load_files()
{
	switch (platType)
	{
	case 1:
		plat1 = load_image( "Images/plat1.png" );
		if ( plat1 == NULL)
		{
			return false;
		}
		break;
	case 2:
		plat2 = load_image( "Images/plat2.png" );
		if ( plat2 == NULL)
		{
			return false;
		}
		break;
	case 3:
		plat3 = load_image( "Images/plat3.png" );
		if ( plat3 == NULL)
		{
			return false;
		}
		break;
	default:
		//Shouldn't happen
		break;
	}

	return true;
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
	case 4: //type
		ret = platType;
		break;
	default: // defualt is x value
		ret = plat.x;
		break;
	}
	return ret;
}

SDL_Surface *Platform::Return(int val)
{
	SDL_Surface *image = NULL;
	switch (val)
	{
	case 1:
		image = plat1;
		break;
	case 2:
		image = plat2;
		break;
	case 3:
		image = plat3;
		break;
	default:
		break;
	}
	return image;
}

void Platform::show(Window& aWindow)
{
	aWindow.apply_surface( plat.x, plat.y, plat1, 1, &camera);
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

Triangle::Triangle()
{
	//Platform default hitboxes
	plat.x = 0;
	plat.y = 750;
	plat.w = 400;
	plat.h = 100;
	//Initialize variables for loading image
	//plat1 = NULL;
	platType = 1;

	load_files();
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
	/*
	bool hit = false, touchFeet = false, touchHead=false, touchLeft=false, touchRight=false;
	int edge = 0, type = 0;
	SDL_Rect thisPlat = this->plat;
	SDL_Rect hitbox = player.Read_rect(0);
	SDL_Rect feet = player.Read_rect(1);
	SDL_Rect head = {hitbox.x,hitbox.y-1,hitbox.w,hitbox.h/4};
	SDL_Rect sideLeft = {hitbox.x-1,hitbox.y,hitbox.w/2+2,hitbox.h/2};
	SDL_Rect sideRight = {hitbox.x+(hitbox.w/2),hitbox.y,hitbox.w/2+2,hitbox.h/2};

	//check if the probes are colliding
	hit = check_collision(thisPlat,hitbox);
	touchFeet = check_collision(thisPlat,feet);
	touchHead = check_collision(thisPlat,head);
	touchLeft = check_collision(thisPlat,sideLeft);
	touchRight = check_collision(thisPlat,sideRight);

	//detect a side collision and give appropriate edge
	if(touchRight == true && touchLeft == false){
		edge = thisPlat.x;
	}
	if(touchRight == false && touchLeft == true){
		edge = thisPlat.x + thisPlat.w;
	}
	if(hit == true && touchFeet == true){
		edge = thisPlat.y;
	}
	player.Collide_Response(touchFeet,touchHead, touchRight, touchLeft, edge, this->plat);
	if(hit == true || touchFeet == true || touchRight == true || touchLeft == true){
		return true;
	}
	else{
		return false;
	}*/
}