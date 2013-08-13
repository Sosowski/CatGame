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
		edge = thisPlat.x;
	}
	if(hit == true && touchFeet == true){
		edge = thisPlat.y;
	}
	player.Collide_Response(hit,touchFeet,touchHead, touchRight, touchLeft, edge, this->plat);
	if(hit == true || touchFeet == true || touchRight == true || touchLeft == true){
		return true;
	}
	else{
		return false;
	}
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
		edge = thisPlat.x;
	}
	if(hit == true && touchFeet == true){
		edge = thisPlat.y;
	}
	player.Collide_Response(hit,touchFeet,touchHead, touchRight, touchLeft, edge, this->plat);
	if(hit == true || touchFeet == true || touchRight == true || touchLeft == true){
		return true;
	}
	else{
		return false;
	}
}